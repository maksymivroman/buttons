#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <DNSServer.h>

#include "ButtonOTAUpdate/AsyncOtaUpdate.h"
#include "Global/Global.hpp"
#include "Global/Version.h"
#include "HTMLPage/html-page.hpp"
#include "LEDService/LEDService.h"
#include "SettingsService/SettingsService.h"
#include "NetworkService/NetworkService.h"
#include "HTMLComponentBuilder/HTMLComponentBuilder.h"
#include "EventsService/EventsService.h"
#include "SoundService/SoundService.h"
#include "TelegramIntegration/TelegramIntegration.h"
#include "TasksHandler/ButtonTask.h"
#include "Logger/Logger.h"
#include "Keystore/Keystore.h"
#include "Statistic/Statistic.h"
#include "ExternalInterface/ExternalInterfaceService.h"
#include "ButtonState/ButtonState.h"

#include "AsyncJson.h"
#include "ArduinoJson.h"

const int bluePin = 12;
const int greenPin = 13;
const int redPin = 14;
const int buttonPin = 5;
const int buzzerPin = 4;

bool requiredToFind, requiredRestart, requiredEepromClear, requiredToTriggerButton, requiredToFormatFS, sendEventsOnKeystoreChange, requiredToUpdateEvents;
bool externalInterface_ledActive, externalInterface_buzzActive;

// this const used for handle POST message type
const char *FIND_ME = "FIND";
const char *SAVE_SETTINGS = "SAVE";
const char *CLEAR_EEPROM = "CLEAR_EEPROM";
const char *TRIGGER_BUTTON = "TRIGGER_BUTTON";
const char *FORMAT_FS = "FORMAT_FS";
const char *ID = "ID";

String integrationMessageToSend = "";

unsigned long timeToExecuteTask = 0;

const char *hotspotPass = "12345678";

Version currentFWVersion(1,3,9, true);

ButtonState buttonState;
LEDService ledService;
SettingsService buttonSettings;
AsyncWebServer server(80);
NetworkService networkService;
HTMLComponentBuilder htmlComponent;
SoundService notifier(buzzerPin);
EventsService eventService;
TelegramIntegration telegramBot;
AsyncOtaUpdate ButtonOTAUpdate;
Statistic statistic;
Keystore keystore(20);
/**Do not set max logs items more than 20,
 * it will cause heap overflow when using Telegram integration (BearSSL requires more than 12k of RAM) */
Logger logger(115200, 20);

ButtonTask RestartTask, EepromClearTask, RequiredToFindTask,
        SendEventsTask, IntegrationTask, CheckConnectionTask(true), ToggleStateTask(false),
        FormatFSTask, OnKeystoreUpdateTask, UpdateEventsWithKeystoreTask, ResetExternalStateTask;

ButtonIntervalTask ChangeLedStateITask, ChangeSoundStateITask;

String components(const String &ref) {
    return htmlComponent.componentById(ref);
}

void restart() {
    optimistic_yield(1000);
    ledService.lightOnRed(true);
    notifier.onRestart();
    EspClass::restart();
}

void setup() {
    pinMode(buttonPin, INPUT);
    ledService.pinConfig(redPin, greenPin, bluePin);
    ledService.blinkDone();

    buttonSettings.loadButtonEepromSettings();
    buttonSettings.handleVersionChange(currentFWVersion.uint_version(), currentFWVersion.EEPROMStructureChanged());

    if (buttonSettings.loggerEnabled()) {
        logger.start(buttonSettings.loggerLevel());
        logger.log("BUTTON CURRENT FW: " , currentFWVersion.str_fullVersion());
    }

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

    String eventsData = *buttonSettings.events();

    WiFiCONFIG wiFiConnDetails = buttonSettings.getWiFiConnDetails();
    EEPROM_SETTINGS configuration = buttonSettings.getButtonConfig();
    INTEGRATIONSETTINGS integrationConfig = buttonSettings.integrationSettings();
    KEYSTORESETTINGS keystoreSettings = buttonSettings.keystoreSettings();
    NETWORKLIST wiFiList;
    wiFiList = networkService.WiFiList();
    notifier.useSound = buttonSettings.useSoundNotification();

    if (digitalRead(buttonPin) == 1) {
        ledService.blinkWarn();
        const char *networkSsid =  buttonSettings.customHotspotSsid();
        networkService.ButtonHotspot(true, networkSsid, hotspotPass);
    } else {
        ledService.lightOnBlue(true);
        networkService.ConnectToWiFi(wiFiConnDetails.ssid,wiFiConnDetails.password);
    }

    ledService.blinkPrimary();

    htmlComponent.setHtmlPageData(wiFiConnDetails.ssid, wiFiConnDetails.password, eventsData, wiFiList, configuration, integrationConfig,
                                  networkService.isConnectedToWiFi());
    eventService.SetEvents(eventsData);

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send_P(200, "text/html", index_html, components);
    });

    server.on("/logs", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send_P(200, "text/html", logs_page);
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
        String responseData = "done";
        if (params > 0) {
            AsyncWebParameter *param = request->getParam(0);
            String paramName = param->name().c_str();
            String paramMessage = param->value().c_str();

            logger.log("[MAIN->HTTP_POST] ", paramName);
            logger.logSerial("[MAIN->HTTP_POST] ", paramName, ": ", paramMessage);

            if (paramName == FIND_ME) {
                requiredToFind = true;
            } else if (paramName == TRIGGER_BUTTON) {
                buttonSettings.remoteButtonTriggering() ? requiredToTriggerButton = true : responseCode = 403;
            } else if (paramName == CLEAR_EEPROM) {
                requiredEepromClear = true;
            } else if (paramName == SAVE_SETTINGS) {
                buttonSettings.saveSettings(paramMessage);
                requiredRestart = true;
            } else if (paramName == FORMAT_FS) {
                //TODO handle diff params value to start different system tools. Rename FORMAT_FS
                requiredToFormatFS = true;
                requiredRestart = true;
            } else if (paramName == ID) {
                responseData = WiFi.macAddress().c_str();
            } else {
                responseCode = 418;
            }
        }
        logger.log("[MAIN->HTTP_POST]-> Return response code: ", responseCode);
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
                if (keystoreSettings.sendEventsOnUpdate) sendEventsOnKeystoreChange = true;
                requiredToUpdateEvents = true;
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

    if(buttonSettings.useTelegramIntegration()) {
        telegramBot.configureTelegramIntegration(buttonSettings.integrationSettings());
        eventService.telegramBotRef = &telegramBot;
        server.on("/integration", HTTP_GET, [](AsyncWebServerRequest *request) {
            if (request->hasParam("data")) {
                integrationMessageToSend += buttonSettings.integrationSettings().tPrefix;
                integrationMessageToSend += request->getParam("data")->value();
                integrationMessageToSend += buttonSettings.integrationSettings().tSuffix;
                logger.log("[MAIN->HTTP GET -> integration]");
                logger.logSerial("[HTTP GET -> integration message]: ", integrationMessageToSend);
            }
            else {logger.log("[MAIN->HTTP GET -> integration]: wrong GET data");}
            request->send_P(200, "text/html", "OK");
        });
    }

    const bool serverEnabled = !networkService.isConnectedToWiFi() || (buttonSettings.clientWebAccessEnabled() &&
                                                                       networkService.isConnectedToWiFi());
    if ( serverEnabled ) {
        if(!networkService.isConnectedToWiFi() || (buttonSettings.otaUpdateOnClientMode() &&
                                                   networkService.isConnectedToWiFi())) {
            ButtonOTAUpdate.setID(buttonSettings.customHotspotSsid());
            ButtonOTAUpdate.begin(&server);
        }
        server.begin(); }

    ledService.idle(buttonSettings.saveLastState(), buttonState.getToggleMode());
}

void loop() {
    delay(100);

    //TODO move to interruption
    buttonState.setState(digitalRead(buttonPin));

    ToggleStateTask(buttonState.isPressed(),[]() {
        auto state = buttonState.toggleState();
        if (buttonSettings.restoreLastStateOnLoad()) buttonSettings.setLastState(state);
        ledService.idle(state);}, !buttonSettings.saveLastState());

    UpdateEventsWithKeystoreTask(requiredToUpdateEvents, [](){
        ledService.updateKeystoreProgress(true);
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
        ledService.updateKeystoreProgress(false);
        requiredToUpdateEvents = false;
        if (buttonSettings.statisticLevel() == 1) statistic.sendStat("Keystore update");
    });

    SendEventsTask((buttonState.isPressed() || requiredToTriggerButton), [](){
        if (requiredToTriggerButton) notifier.onRemoteTrigger();

        externalInterface_ledActive = false;
        externalInterface_buzzActive = false;
        ledService.eventsSendInProgress(true);
        eventService.SendEvents(buttonState.getEventTrigger(buttonSettings.saveLastState()));
        ledService.eventsSendInProgress(false);

        if (requiredToTriggerButton) {
            requiredToTriggerButton = !requiredToTriggerButton;
            if (buttonSettings.statisticLevel() == 1) statistic.sendStat("Send events. Triggered by: remote");
        } else {
            statistic.sendStat("Send events. Triggered by: physical");
        }
    },networkService.isAPMode());

    RequiredToFindTask(requiredToFind , [](){
        notifier.onFindMe();
        ledService.findMe();
        requiredToFind = !requiredToFind;
    });

    EepromClearTask(requiredEepromClear,  [](){
        buttonSettings.clearEeprom();
        requiredRestart = true;
    });

    FormatFSTask(requiredToFormatFS, [](){
        ledService.onFormatFS();
        buttonSettings.formatFS();
        requiredRestart = true;
    });

    IntegrationTask(integrationMessageToSend.length() != 0, [](){
        notifier.onIntegrationMessage();
        telegramBot.sendMessage(integrationMessageToSend);
        integrationMessageToSend = "";
        if (buttonSettings.statisticLevel() == 1) statistic.sendStat("Integration: send message to telegram.");
    });

    OnKeystoreUpdateTask(
            sendEventsOnKeystoreChange, []() {
                ledService.updateKeystoreProgress(true);
                eventService.SendEvents(KEYSTORE_UPDATE);
                sendEventsOnKeystoreChange = !sendEventsOnKeystoreChange;
                timeToExecuteTask = 0;
                ledService.updateKeystoreProgress(false);
            },
            [](){},
            []() -> bool { return millis() < timeToExecuteTask; }
    );

    CheckConnectionTask(
            networkService.isConnectedToWiFi(),
            [](){ logger.log("[CheckConnectionTask]: Connected"); ledService.idle(buttonSettings.saveLastState(), buttonState.getToggleMode()); },
            [](){ logger.log("[CheckConnectionTask]: Disconnected"); ledService.onNoConnection(); },
            networkService.isAPMode()
            );

    ChangeLedStateITask(1000, []() {
        ledService.onExternalInterfaceProgress(true);
    }, !externalInterface_ledActive);

    ChangeSoundStateITask(10000, []() {
        notifier.onExternal();
    }, !externalInterface_buzzActive);

    ResetExternalStateTask(
            !externalInterface_ledActive,
            [](){ledService.idle(buttonSettings.saveLastState(), buttonState.getToggleMode());});

    RestartTask((requiredRestart || ButtonOTAUpdate.requireToRestart), restart);

}
