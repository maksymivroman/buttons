#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <DNSServer.h>

#include "AsyncOtaUpdate.h"
#include "GlobalConfig.hpp"
#include "html-page.hpp"
#include "LEDService.h"
#include "SettingsService.h"
#include "NetworkService.h"
#include "HTMLComponentBuilder.h"
#include "EventsService.h"
#include "SoundService.h"
#include "TelegramIntegration.h"
#include "Tasks/ButtonTask.h"

const int bluePin = 12;
const int greenPin = 13;
const int redPin = 14;
const int buttonPin = 5;
const int buzzerPin = 4;

bool requiredToFind, requiredRestart, requiredEepromClear, requiredToTriggerButton;

// this const used for handle POST message type
const char *FIND_ME = "FIND";
const char *SAVE_SETTINGS = "SAVE";
const char *CLEAR_EEPROM = "CLEAR_EEPROM";
const char *TRIGGER_BUTTON = "TRIGGER_BUTTON";

String integrationMessageToSend = "";

const char *hotspotPass = "12345678";

LEDService ledService;
SettingsService buttonSettings;
AsyncWebServer server(80);
NetworkService networkService;
HTMLComponentBuilder htmlComponent;
SoundService notifier(buzzerPin);
EventsService eventService;
TelegramIntegration telegramBot;
AsyncOtaUpdate ButtonOTAUpdate;

ButtonTask RestartTask, EepromClearTask, RequiredToFindTask, SendEventsTask, IntegrationTask, CheckConnectionTask(true);

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
    Serial.begin(115200);
    Serial.println();
    Serial.println( "BUTTON CURRENT FW: " + currentFirmwareVersion);
    ledService.pinConfig(redPin, greenPin, bluePin);
    pinMode(buttonPin, INPUT);
    ledService.blinkDone();

    String eventsData = buttonSettings.loadEvents();
    buttonSettings.loadButtonEepromSettings();

    WiFiCONFIG wiFiConnDetails = buttonSettings.getWiFiConnDetails();
    EEPROMSETTINGS configuration = buttonSettings.getButtonConfig();
    INTEGRATIONSETTINGS integrationConfig = buttonSettings.integrationSettings();
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

    server.on("/", HTTP_POST, [](AsyncWebServerRequest *request) {
        unsigned int params = request->params();
        Serial.print("[MAIN->HTTP_POST] params count: "); Serial.println(params);
        int responseCode = 200;
        if (params > 0) {
            AsyncWebParameter *param = request->getParam(0);
            String paramName = param->name().c_str();
            String paramMessage = param->value().c_str();

            Serial.print("[MAIN->HTTP_POST] "); Serial.print(paramName); Serial.print(": "); Serial.println(paramMessage);

            if (paramName == FIND_ME) {
                requiredToFind = true;
            } else if (paramName == TRIGGER_BUTTON) {
                buttonSettings.remoteButtonTriggering() ? requiredToTriggerButton = true : responseCode = 403;
            } else if (paramName == CLEAR_EEPROM) {
                requiredEepromClear = true;
            } else if (paramName == SAVE_SETTINGS) {
                buttonSettings.saveSettings(paramMessage);
                requiredRestart = true;
            } else {
                responseCode = 418;
            }
        }
        Serial.print("[MAIN->HTTP_POST->STATUS] "); Serial.println(responseCode);
        request->send(responseCode, "text/html", "done");
    });

    if(buttonSettings.useTelegramIntegration()) {
        telegramBot.configureTelegramIntegration(buttonSettings.integrationSettings());
        eventService.telegramBotRef = &telegramBot;
        server.on("/integration", HTTP_GET, [](AsyncWebServerRequest *request) {
            if (request->hasParam("data")) {
                integrationMessageToSend += buttonSettings.integrationSettings().tPrefix;
                integrationMessageToSend += request->getParam("data")->value();
                integrationMessageToSend += buttonSettings.integrationSettings().tSuffix;
                Serial.print("[HTTP GET -> integration]: "); Serial.println(integrationMessageToSend);
            }
            else {Serial.print("[HTTP GET -> integration]: "); Serial.println("wrong GET data");}
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

    ledService.lightOnGreen(true);
}

void loop() {
    delay(100);

    SendEventsTask((digitalRead(buttonPin) == 1 || requiredToTriggerButton), [](){
        if (requiredToTriggerButton) {
            requiredToTriggerButton =!requiredToTriggerButton;
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

    RestartTask((requiredRestart || ButtonOTAUpdate.requireToRestart), restart);

    IntegrationTask(integrationMessageToSend.length() != 0, [](){
        notifier.onIntegrationMessage();
        telegramBot.sendMessage(integrationMessageToSend);
        integrationMessageToSend = "";
    });

    CheckConnectionTask(
            networkService.isConnectedToWiFi(),
            [](){ Serial.println("[CheckConnectionTask]: Connected"); ledService.lightOnGreen(true); },
            [](){ Serial.println("[CheckConnectionTask]: Disconnected"); ledService.lightOnRed(true); },
            networkService.isAPMode()
            );

}
