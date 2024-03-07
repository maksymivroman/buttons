#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <DNSServer.h>

#include "Global/Global.hpp"
#include "Global/Version.h"
#include "HTMLPage/html-page.hpp"
#include "LEDService/LEDService.h"
#include "SettingsService/SettingsService.h"
#include "NetworkService/NetworkService.h"
#include "HTMLComponentBuilder/HTMLComponentBuilder.h"
#include "EventsService/EventsService.h"
#include "SoundService/SoundService.h"
#include "TasksHandler/ButtonTask.h"
#include "Logger/Logger.h"

#include "AsyncJson.h"
#include "ArduinoJson.h"

const int bluePin = 12;
const int greenPin = 13;
const int redPin = 14;
const int buttonPin = 5;
const int buzzerPin = 4;

bool requiredToFind, requiredRestart, requiredEepromClear, requiredToTriggerButton, requiredToFormatFS;

// this const used for handle POST message type
const char *FIND_ME = "FIND";
const char *SAVE_SETTINGS = "SAVE";
const char *CLEAR_EEPROM = "CLEAR_EEPROM";
const char *TRIGGER_BUTTON = "TRIGGER_BUTTON";
const char *FORMAT_FS = "FORMAT_FS";
const char *ID = "ID";

const char *hotspotPass = "12345678";

Version currentFWVersion(1,3,0, false);

LEDService ledService;
SettingsService buttonSettings;
AsyncWebServer server(80);
NetworkService networkService;
HTMLComponentBuilder htmlComponent;
SoundService notifier(buzzerPin);
EventsService eventService;
Logger logger(115200, 20);

ButtonTask RestartTask, EepromClearTask, RequiredToFindTask,
        SendEventsTask, CheckConnectionTask(true),
        FormatFSTask;

String components(const String &ref) {
    return htmlComponent.componentById(ref);
}

[[noreturn]] void restart() {
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
        logger.log("SANITIZER CURRENT FW: " , currentFWVersion.str_fullVersion());
    }

    buttonSettings.loadEvents();

    String eventsData = *buttonSettings.events();

    WiFiCONFIG wiFiConnDetails = buttonSettings.getWiFiConnDetails();
    EEPROMSETTINGS configuration = buttonSettings.getButtonConfig();
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

    htmlComponent.setHtmlPageData(wiFiConnDetails.ssid, wiFiConnDetails.password, eventsData, wiFiList, configuration,
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

    const bool serverEnabled = !networkService.isConnectedToWiFi() || (buttonSettings.clientWebAccessEnabled() &&
                                                                       networkService.isConnectedToWiFi());
    if ( serverEnabled ) { server.begin(); }

    ledService.lightOnGreen(true);
}

void loop() {
    delay(100);

    SendEventsTask((digitalRead(buttonPin) == 1 || requiredToTriggerButton), [](){
        if (requiredToTriggerButton) {
            requiredToTriggerButton = !requiredToTriggerButton;
            notifier.onRemoteTrigger();
        }
        ledService.eventsSendInProgress(true);
        eventService.SendEvents();
        ledService.eventsSendInProgress(false);
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
        ledService.lightOnRed(true);
        buttonSettings.formatFS();
    });

    CheckConnectionTask(
            networkService.isConnectedToWiFi(),
            [](){ logger.log("[CheckConnectionTask]: Connected"); ledService.lightOnGreen(true); },
            [](){ logger.log("[CheckConnectionTask]: Disconnected"); ledService.lightOnRed(true); },
            networkService.isAPMode()
            );

    RestartTask(requiredRestart, restart);

}
