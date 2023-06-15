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

const int bluePin = 12;
const int greenPin = 13;
const int redPin = 14;
const int buttonPin = 5;
const int buzzerPin = 4;

bool requiredToFind, requiredRestart, requiredEepromClear;
String messageToSend = "";

const char *hotspotPass = "12345678";

LEDService ledService;
SettingsService buttonSettings;
AsyncWebServer server(80);
NetworkService networkService;
HTMLComponentBuilder htmlComponent;
SoundService notifier(buzzerPin);
EventsService* eventService = new EventsService();
TelegramIntegration telegramBot;
AsyncOtaUpdate ButtonOTAUpdate;

String components(const String &ref) {
    return htmlComponent.componentById(ref);
}

[[noreturn]] void restart() {
    ledService.lightOnRed(true);
    notifier.onRestart();
    EspClass::restart();
}

void setup() {
    Serial.begin(115200);
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

    htmlComponent.setHtmlPageData(wiFiConnDetails.ssid, wiFiConnDetails.password, eventsData, wiFiList, configuration, integrationConfig);
    eventService->SetEvents(eventsData);

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send_P(200, "text/html", index_html, components);
    });

    server.on("/", HTTP_POST, [](AsyncWebServerRequest *request) {
        int params = request->params();
        Serial.print("[MAIN->HTTP_POST] "); Serial.println(params);
        if (params >= 0) {
            AsyncWebParameter *param = request->getParam(0);
            String postMessage = param->value().c_str();
            Serial.print("[MAIN->HTTP_POST] "); Serial.println(postMessage);
            const char *findMe = "find";
            const char *clearEEPROM = "clearEEPROM";
            if (postMessage == findMe) {
                requiredToFind = true;
            } else if (postMessage == clearEEPROM) {
                requiredEepromClear = true;
            }  else {
                buttonSettings.saveSettings(postMessage);
                requiredRestart = true;
            }
        }
        request->send(200, "text/html", "done");
    });

    if(buttonSettings.useTelegramIntegration()) {
        telegramBot.configureTelegramIntegration(buttonSettings.integrationSettings());
        server.on("/integration", HTTP_GET, [](AsyncWebServerRequest *request) {
            if (request->hasParam("data")) {
                messageToSend += buttonSettings.integrationSettings().tPrefix;
                messageToSend += request->getParam("data")->value();
                messageToSend += buttonSettings.integrationSettings().tSuffix;
                Serial.print("[HTTP GET -> integration]: "); Serial.println(messageToSend);
            }
            else {Serial.print("[HTTP GET -> integration]: "); Serial.println("wrong GET data");}
            request->send_P(200, "text/html", "OK");
        });
    }

    const bool serverEnabled = !networkService.isClientMode() || (buttonSettings.clientWebAccessEnabled() && networkService.isClientMode());
    if ( serverEnabled ) {
        //TODO check client mode
        if(buttonSettings.otaUpdateOnClientMode()) {
            ButtonOTAUpdate.setID(buttonSettings.customHotspotSsid());
            ButtonOTAUpdate.begin(&server);
        }
        server.begin(); }

    ledService.lightOnGreen(true);
}

void loop() {
    delay(100);

    if (digitalRead(buttonPin) == 1) {
        ledService.eventsSendInProgress(true);
        eventService->SendEvents();
        ledService.eventsSendInProgress(false);
    }

    if(requiredToFind) {
        notifier.onFindMe();
        ledService.findMe();
        requiredToFind = !requiredToFind;
    }

    if(requiredEepromClear) {
        buttonSettings.clearEeprom();
        requiredRestart = true;
    }

    if(messageToSend.length() != 0) {
        notifier.onIntegrationMessage();
        telegramBot.sendMessage(messageToSend);
        messageToSend = "";
    }

    if(requiredRestart || ButtonOTAUpdate.requireToRestart) {
        restart();
    }
}
