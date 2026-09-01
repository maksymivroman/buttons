#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <DNSServer.h>

#include "ButtonOTAUpdate/AsyncOtaUpdate.h"
#include "Global/Global.hpp"
#include "Global/Version.h"
#include "Global/SettingsOptions.hpp"
#include "HTMLPage/index-html-page.hpp"
#include "HTMLPage/server-editor-page.hpp"
#include "HTMLPage/flags-html-page.hpp"
#include "LEDService/LEDService.h"
#include "SettingsService/SettingsService.h"
#include "NetworkService/NetworkService.h"
#include "HTMLComponentBuilder/HTMLComponentBuilder.h"
#include "EventsService/EventsService.h"
#include "SoundService/SoundService.h"
#include "TasksHandler/ButtonTask.h"
#include "Logger/Logger.h"
#include "Keystore/Keystore.h"
#include "Statistic/Statistic.h"
#include "ExternalInterface/ExternalInterfaceService.h"
#include "ButtonState/ButtonState.h"
#include "ButtonTrigger/Trigger.h"
#include "ButtonFlags/ButtonFlags.h"
#include "ApiResponse/ApiResponse.h"

#include "AsyncJson.h"
#include "ArduinoJson.h"

const int bluePin = 12;
const int greenPin = 13;
const int redPin = 14;
const int buttonPin = 5;
const int buzzerPin = 4;

bool externalInterface_ledActive, externalInterface_buzzActive;

Trigger RequiredToTriggerButton, RequiredToFind, RequiredEepromClear, RequiredToFormatFS, RequiredRestart, RequiredToUpdateEvents, SendEventsOnKeystoreChange;

// this const used for handle POST message type
const char *FIND_ME = "FIND";
const char *SAVE_SETTINGS = "SETTINGS";
const char *SAVE_EVENTS = "EVENTS";
const char *CLEAR_EEPROM = "CLEAR_EEPROM";
const char *TRIGGER_BUTTON = "TRIGGER_BUTTON";
const char *FORMAT_FS = "FORMAT_FS";
const char *ID = "ID";

unsigned long timeToExecuteTask = 0;

const char *hotspotPass = "12345678";

Version currentFWVersion(1,6,0, true);

ButtonState buttonState;
LEDService ledService;
SettingsService buttonSettings;
AsyncWebServer server(80);
NetworkService networkService;
HTMLComponentBuilder htmlComponent(buttonState);
SoundService notifier(buzzerPin);
EventsService eventService;
AsyncOtaUpdate ButtonOTAUpdate;
Statistic statistic;
Keystore keystore(20);
Logger logger(115200, 20);

ButtonTask RestartTask, EepromClearTask, RequiredToFindTask,
        SendEventsTask, CheckConnectionTask(true), ToggleStateTask(false),
        FormatFSTask, OnKeystoreUpdateTask, UpdateEventsWithKeystoreTask, ResetExternalStateTask;

ButtonIntervalTask MainITask, MainStateITask;
ButtonIntervalTask ChangeLedStateITask, ChangeSoundStateITask, ConnectToWiFiITask;

String components(const String &ref) {
    return htmlComponent.componentById(ref);
}

void restart() {
    optimistic_yield(1000);
    ledService.setLedAction(ACTIONS::WARN);
    notifier.onRestart();
    EspClass::restart();
}

void setup() {
    pinMode(buttonPin, INPUT);
    ledService.pinConfig(redPin, greenPin, bluePin);

    buttonSettings.loadButtonEepromSettings();
    buttonSettings.loadButtonFlags();
    if (buttonSettings.overrideLedConfig()) ledService.applyLedMap(buttonSettings.ledMap());

    ledService.setLedAction(ACTIONS::LOADING, true);

    auto isVersionChanged = buttonSettings.handleVersionChange(currentFWVersion.uint_version(), currentFWVersion.EEPROMStructureChanged());
    auto serialEventsEnabled = buttonSettings.serialEvents();

    if (buttonSettings.loggerEnabled()) {
        if (serialEventsEnabled && (buttonSettings.loggerLevel() == SERIAL_AND_LOCAL || buttonSettings.loggerLevel() == LOGGER_SERIAL)) {
            logger.start(LOGGER_LOCAL);
            Serial.begin(115200);
        } else {
            logger.start(buttonSettings.loggerLevel());
        }
    } else {
        if (serialEventsEnabled) Serial.begin(115200);
    }

    logger.log("BUTTON CURRENT FW: " , currentFWVersion.str_fullVersion());
    logger.log("BUTTON CURRENT HW: " , currentFWVersion.getBoardName());

    auto timeZoneId = GET_TIMEZONE(buttonSettings.timeZoneId());
    logger.log("[MAIN:INIT] Config time for time zone ", timeZoneId);
    configTime(timeZoneId.c_str(), "pool.ntp.org", "time.nist.gov");

    if (buttonSettings.statisticEnabled()) {
        statistic.initStat(buttonSettings.statisticApi(),STAT_HTTP_POST, buttonSettings.statisticLevel());
    }

    if (buttonSettings.saveLastState()) {
        logger.log("[MAIN:INIT] Button Toggle mode enabled");
        buttonSettings.loadButtonDynamicProps();
        if(buttonSettings.restoreLastStateOnLoad()) {
            auto state = static_cast<BUTTON_STATE>(buttonSettings.getLastStatePressed());
            buttonState.setToggleState(state);
            logger.log("[MAIN:INIT] Button Toggle mode restored to : ", state);
        }
    }

    buttonSettings.loadEvents();

    auto eventsData = buttonSettings.events();
    bool serialEvents = buttonSettings.serialEvents();

    KEYSTORESETTINGS keystoreSettings = buttonSettings.keystoreSettings();
    notifier.useSound = buttonSettings.useSoundNotification();
    networkService.setWiFiMode(buttonSettings.wiFiMode());

    auto opMode = static_cast<OPERATION_MODE>(digitalRead(buttonPin));
    buttonState.setOperationMode(isVersionChanged ? SETUP : opMode);

    if(buttonState.isSetupOperationMode()) {
        if(isVersionChanged) notifier.onFWUpdate();
        ledService.setLedAction(ACTIONS::WARN, true);
        const char *networkSsid =  buttonSettings.customHotspotSsid();
        networkService.ButtonHotspot(true, networkSsid, hotspotPass);
        WiFi.scanNetworks(true);
    }

    ledService.setLedAction(ACTIONS::DONE, true);
    eventService.SetEvents(*eventsData, serialEvents);

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send_P(200, "text/html", index_html, components);
    });

    server.on("/logs", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send_P(200, "text/html", logs_page, components);
    });

    server.on("/events", HTTP_GET, [](AsyncWebServerRequest *request) {
        ApiResponse::sendSuccess(request, [](JsonObject &result) {
            buttonSettings.getEventsJson(result);
        }, 2048);
    });

    server.on("/settings", HTTP_GET, [](AsyncWebServerRequest *request) {
        ApiResponse::sendSuccess(request, [](JsonObject &result) {
            buttonSettings.getSettingsJson(result);
        }, 2560);
    });

    server.on("/networks", HTTP_GET, [](AsyncWebServerRequest *request) {
        ApiResponse::sendSuccess(request, [](JsonObject &result) {
            networkService.getNetworksJson(result);
        }, 1536);
    });

    server.on("/status", HTTP_GET, [](AsyncWebServerRequest *request) {
        ApiResponse::sendSuccess(request, [](JsonObject &result) {
            result["fwVersion"] = currentFWVersion.str_version();
            result["hwVersion"] = currentFWVersion.getBoardName();
            result["mac"] = buttonSettings.macAddress();
            result["ip"] = networkService.ipAddress();
            result["heap"] = ESP.getFreeHeap();
            result["ksKeys"] = keystore.currentItemsCount();
            result["toggleState"] = buttonState.getToggleMode() ? "PRESSED" : "RELEASED";
            result["wiFiMode"] = networkService.getWiFIMode();

            EEPROM_FLAGS flags = buttonSettings.buttonFlags();
            JsonObject rgbFlags = result.createNestedObject("rgbFlags");
            rgbFlags["rDisabled"] = flags.ledRDisabled;
            rgbFlags["gDisabled"] = flags.ledGDisabled;
            rgbFlags["bDisabled"] = flags.ledBDisabled;
        }, 1024);
    });

    server.on("/logsData", HTTP_GET, [](AsyncWebServerRequest *request) {
        auto * response = new AsyncJsonResponse(false, 8192);
        JsonObject root = response->getRoot();
        const unsigned int logsCount = logger.logs().size();
        for (size_t i = 0; i < logsCount; ++i) {
            root[std::to_string(i)] = logger.logs()[i];
        }
        response->setLength();
        request->send(response);
    });

    server.on("/", HTTP_POST, [](AsyncWebServerRequest *request) {
        unsigned int params = request->params();
        logger.log("[MAIN->HTTP_POST] params count: ", std::to_string(params).c_str());
        int responseCode = 200;
        String responseData = "OK";
        if (params > 0) {
            AsyncWebParameter *param = request->getParam(0);
            String paramName = param->name().c_str();
            String paramMessage = param->value().c_str();

            logger.log("[MAIN->HTTP_POST] ", paramName);
            logger.logSerial("[MAIN->HTTP_POST] ", paramName, ": ", paramMessage);

            RequiredToTriggerButton.setIf(paramName == TRIGGER_BUTTON && buttonSettings.remoteButtonTriggering());
            RequiredToFind.setIf(paramName == FIND_ME);
            RequiredEepromClear.setIf(paramName == CLEAR_EEPROM);
            RequiredToFormatFS.setIf(paramName == FORMAT_FS);

            if (paramName == SAVE_SETTINGS) {
                buttonSettings.saveSettings(paramMessage);
                RequiredRestart.set();
            } else if (paramName == SAVE_EVENTS) {
                buttonSettings.saveEvents(paramMessage);
                eventService.SetEvents(paramMessage);
            }else if (paramName == ID) {
                responseData = WiFi.macAddress().c_str();
            } else if (!(RequiredToTriggerButton || RequiredToFind || RequiredEepromClear || RequiredToFormatFS)){
                responseCode = 418;
                responseData = "Bad request";
            }
        }
        logger.log("[MAIN->HTTP_POST]-> Return response code: ", responseCode);
        request->send(responseCode, "text/html", responseData);
    });

    server.on("/flags", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send_P(200, "text/html", flags_page, components);
    });

    server.on("/flagsData", HTTP_GET, [](AsyncWebServerRequest *request) {
        ApiResponse::sendSuccess(request, [](JsonObject &result) {
            EEPROM_FLAGS flags = buttonSettings.buttonFlags();
            result["ledRDisabled"] = flags.ledRDisabled;
            result["ledGDisabled"] = flags.ledGDisabled;
            result["ledBDisabled"] = flags.ledBDisabled;
        }, 512);
    });

    server.on("/flags", HTTP_POST, [](AsyncWebServerRequest *request) {
        unsigned int params = request->params();
        int responseCode = 200;
        String responseData = "OK";
        logger.log("[MAIN->HTTP_POST] params count: ", std::to_string(params).c_str());
        if (params > 0) {
            ButtonFlags bf;
            bool hasValidFlags = false;
            for (unsigned int i = 0; i < params; i++) {
                AsyncWebParameter *param = request->getParam(i);
                String paramName = param->name();
                if (bf.isValidFlag(paramName)) {
                    hasValidFlags = true;
                    break;
                }
            }

            if (hasValidFlags) {
                EEPROM_FLAGS buttonFlags = buttonSettings.buttonFlags();
                for (unsigned int i = 0; i < params; i++) {
                    AsyncWebParameter *param = request->getParam(i);
                    String flagName = param->name();
                    bool value = param->value() == "true" || param->value() == "1";
                    bf.updateFlags(&buttonFlags, flagName, value);
                    logger.log("[MAIN->HTTP_POST] Updating Flag: ", flagName.c_str(), ": ", value);
                }
                buttonSettings.updateFlagsEEPROM(buttonFlags);
                RequiredRestart.set();
                responseData = "Flags updated!";
            } else {
                responseData = "Flags not set!";
            }
        } else {
            responseCode = 400;
            responseData = "Bad request";
        }
        request->send(responseCode, "text/html", responseData);
    });

    server.on("/keystore", HTTP_GET, [keystoreSettings](AsyncWebServerRequest *request) {
        if (keystoreSettings.enabled) {
            unsigned int paramsCount = request->params();
            if (paramsCount != 0) {
                for (unsigned int i = 0; i < paramsCount; i++) {
                    String paramName = request->getParam(i)->name();
                    String paramValue = request->getParam(i)->value();
                    logger.logSerial("[MAIN->HTTP GET][/keystore] receive param - ", paramName,": ", paramValue);
                    if (paramName == "delay" && keystoreSettings.delayEvent) {
                        const unsigned int delayMs = std::stoi(paramValue.c_str());
                        timeToExecuteTask = millis() + delayMs;
                        logger.log("[MAIN->HTTP GET][/keystore] Set task delay(ms): ", delayMs);
                    } else {
                        keystore.addItem(paramName, paramValue);
                    }
                }
                if (keystoreSettings.sendEventsOnUpdate) SendEventsOnKeystoreChange.set();
                RequiredToUpdateEvents.set();
                logger.log("[MAIN->HTTP GET][/keystore]: Update Keystore, total items - ", paramsCount);
                request->send_P(200, "text/html", "OK");
            } else {
                logger.log("[MAIN->HTTP GET][/keystore]: no parameters received");
                request->send_P(400, "text/html", "Bad Request");
            }
        } else {
            logger.log("[MAIN->HTTP GET][/keystore]: Keystore disabled. Forbidden.");
            request->send_P(403, "text/html", "Forbidden");
        }
    });

    server.on("/server/editor", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send_P(200, "text/html", editor_html, components);
    });

    server.on("/server/load-config", HTTP_GET, [](AsyncWebServerRequest *request) {
        SERVER_CONFIG cfg = buttonSettings.serverConfig();
        String json = "{\"path\":\"" + cfg.serverPath + "\"}";
        request->send(200, "application/json", json);
    });

    server.on("/server/load-html", HTTP_GET, [](AsyncWebServerRequest *request) {
        if (SPIFFS.exists(SERVER_HTML_FILE)) {
            request->send(SPIFFS, SERVER_HTML_FILE, "text/plain");
        } else {
            request->send(200, "text/plain", "");
        }
    });

    server.on("/server/upload", HTTP_POST, [](AsyncWebServerRequest *request) {
        request->send(200, "text/plain", "File Uploaded");
    }, [](AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final) {
        if (!index) {
            request->_tempFile = SPIFFS.open(SERVER_HTML_FILE, "w");
            logger.log("Upload Start: %s\n", filename.c_str());
        }

        if (request->_tempFile) {
            request->_tempFile.write(data, len);
        }

        if (final) {
            request->_tempFile.close();
            logger.log("Upload Finished");

            if (request->hasArg("path")) {
                buttonSettings.saveServerConfig(request->arg("path"));
            }
        }
    });

    server.onNotFound([](AsyncWebServerRequest *request) {
        auto useServer = buttonSettings.customServer();
        if (useServer && (request->url() == buttonSettings.serverConfig().serverPath)) {
            if (SPIFFS.exists(SERVER_HTML_FILE)) {
                request->send(SPIFFS, SERVER_HTML_FILE, "text/html");
            } else {
                request->send(200, "text/html", "Upload your HTML first.");
            }
        } else {
            request->send(404);
        }
    });

    if (buttonSettings.remoteStateChangeEnabled()) {
        server.on("/external", HTTP_GET, [](AsyncWebServerRequest *request) {
            unsigned int paramsCount = request->params();
            if (paramsCount != 0) {
                logger.log("[MAIN->HTTP GET][/external]: Update LED/Sound state. Params received: ", paramsCount);
                auto * external = new ExternalInterfaceService();
                external->handleTriggers(request, {
                        {EXTERNAL_LED, externalInterface_ledActive},
                        {EXTERNAL_BEEP, externalInterface_buzzActive}
                });
                delete external;
            }
            logger.log("[MAIN->HTTP_POST]-> Return response code: ", 200);
            request->send_P(200, "text/html", "OK");
        });
    }

    const bool buttonOTAUpdateEnabled = buttonState.isSetupOperationMode() || (buttonState.isRunOperationMode() && buttonSettings.otaUpdateOnClientMode());
    const bool serverEnabled = buttonState.isSetupOperationMode() || (buttonState.isRunOperationMode() && buttonSettings.clientWebAccessEnabled());

    if (serverEnabled) {
        if (buttonOTAUpdateEnabled) {
            ButtonOTAUpdate.setID(buttonSettings.customHotspotSsid());
            ButtonOTAUpdate.begin(&server);
        }
        server.begin();
    }

    auto action = []()->ACTIONS{
        if (buttonSettings.saveLastState() && buttonState.getToggleMode() == PRESSED) return ACTIONS::IDLE_PRESSED;
        return ACTIONS::IDLE_DEFAULT;
    };
    ledService.setLedAction(action());
}

void loop() {
    ConnectToWiFiITask(1000, []() {
        auto config = buttonSettings.getWiFiConnDetails();
        networkService.initConnectionToWiFi(config);
        logger.logSerial("[MAIN] Pending connection to ", config.ssid, " / ", config.password);
    }, buttonState.isSetupOperationMode() || networkService.isConnectedToWiFi());

    CheckConnectionTask(
            networkService.isConnectedToWiFi(),
            [](){ logger.log("[CheckConnectionTask]: Connected. IP: ", networkService.ipAddress());
                auto action = []()->ACTIONS{
                        if (buttonSettings.saveLastState() && buttonState.getToggleMode() == PRESSED) return ACTIONS::IDLE_PRESSED;
                        return ACTIONS::IDLE_DEFAULT;
                };
                ledService.setLedAction(action());
                },
            [](){ logger.log("[CheckConnectionTask]: Disconnected"); ledService.setLedAction(ACTIONS::WARN); },
            buttonState.isSetupOperationMode()
    );

    MainStateITask(100, [&]() {
        //TODO move to interruption
        buttonState.setState(RequiredToTriggerButton || digitalRead(buttonPin));

        ToggleStateTask(buttonState.isPressed(),[]() {
                            auto state = buttonState.toggleToggleState();
                            if (buttonSettings.restoreLastStateOnLoad()) buttonSettings.setLastState(state);
                            ledService.setLedAction(buttonState.getToggleMode() == PRESSED ? ACTIONS::IDLE_PRESSED : ACTIONS::IDLE_DEFAULT);},
                        !buttonSettings.saveLastState());

        SendEventsTask((buttonState.isPressed()), []() {
            if (RequiredToTriggerButton) notifier.onRemoteTrigger();

            externalInterface_ledActive = false;
            externalInterface_buzzActive = false;
            ledService.setLedAction(ACTIONS::SEND_EVENTS, false, false);
            eventService.SendEvents(buttonState.getEventTrigger(buttonSettings.saveLastState()));
            ledService.resetLedAction();

            if (RequiredToTriggerButton) {
                RequiredToTriggerButton.reset();
                if (buttonSettings.statisticLevel() == 1) statistic.sendStat("Send events. Triggered by: remote");
            } else {
                statistic.sendStat("Send events. Triggered by: physical");
            }
        }, false);
    });

    MainITask(100, [&]() {

        UpdateEventsWithKeystoreTask(RequiredToUpdateEvents.get(), []() {
            ledService.setLedAction(ACTIONS::KEYSTORE_UPDATE, false, false);
            auto items = keystore.keystoreItems();
            String events = *buttonSettings.events();
            for (auto &item: items) {
                const auto &key = item.first;
                const auto &value = item.second;
                String constructKey = "$";
                constructKey.concat(key); constructKey.concat("$");
                events.replace(constructKey,value);
            }
            eventService.SetEvents(events);
            ledService.resetLedAction();
            RequiredToUpdateEvents.reset();
            if (buttonSettings.statisticLevel() == 1) statistic.sendStat("Keystore update");
        });

        RequiredToFindTask(RequiredToFind.get(), []() {
            notifier.onFindMe();
            ledService.setLedAction(ACTIONS::WARN, true);
            RequiredToFind.reset();
        });

        EepromClearTask(RequiredEepromClear.get(), []() {
            buttonSettings.clearEeprom();
            RequiredRestart.set();
        });

        FormatFSTask(RequiredToFormatFS.get(), []() {
            ledService.setLedAction(ACTIONS::WARN);
            buttonSettings.formatFS();
            RequiredRestart.set();
        });

        OnKeystoreUpdateTask(
                SendEventsOnKeystoreChange.get(), []() {
                    ledService.setLedAction(ACTIONS::KEYSTORE_UPDATE, false, false);
                    eventService.SendEvents(KEYSTORE_UPDATE);
                    SendEventsOnKeystoreChange.reset();
                    timeToExecuteTask = 0;
                    ledService.resetLedAction();
                },
                []() {},
                []() -> bool { return millis() < timeToExecuteTask; }
        );

        ChangeLedStateITask(1000, []() {
            ledService.setLedAction(ACTIONS::EXTERNAL_INTERFACE);
        }, !externalInterface_ledActive);

        ChangeSoundStateITask(10000, []() {
            notifier.onExternal();
        }, !externalInterface_buzzActive);

        ResetExternalStateTask(
                !externalInterface_ledActive,
                [](){
                    auto action = []()->ACTIONS{
                        if (buttonSettings.saveLastState() && buttonState.getToggleMode() == PRESSED) return ACTIONS::IDLE_PRESSED;
                        return ACTIONS::IDLE_DEFAULT;
                    };
                    ledService.setLedAction(action());
                });

        RestartTask((RequiredRestart || ButtonOTAUpdate.requireToRestart), restart);


    }, buttonState.isRunOperationMode() && !networkService.isConnectedToWiFi());
}
