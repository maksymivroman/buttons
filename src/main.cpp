#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ESP8266HTTPClient.h>
#include "ArduinoJson.h"
#include <EEPROM.h>
#include <DNSServer.h>

#include "LEDService.h"
#include "html-page.hpp"
#include "ButtonWebServer.h"
#include "GlobalConfig.hpp"
#include "SettingsService.h"
#include "NetworkService.h"
#include "HTMLComponentBuilder.h"
#include "EventsService.h"

const int bluePin = 12;
const int greenPin = 13;
const int redPin = 14;
const int buttonPin = 5;

const char *hotspotSsid = "BUTTON_CONFIG";
const char *hotspotPass = "12345678";

LEDService ledService;
SettingsService buttonSettings;
AsyncWebServer server(80);
NetworkService networkService;
HTMLComponentBuilder htmlComponent;
EventsService* eventService = new EventsService();

String components(const String &ref) {
    return htmlComponent.componentById(ref);
}

void setup() {
    Serial.begin(115200);
    ledService.pinConfig(redPin, greenPin, bluePin);
    pinMode(buttonPin, INPUT);
    ledService.blinkDone();

    String eventsData = buttonSettings.loadEvents();
    WiFiCONFIG wiFiConnDetails = buttonSettings.loadWiFiSettings();

    NETWORKLIST wiFiList;
    wiFiList = networkService.WiFiList();

    if (digitalRead(buttonPin) == 0) {
        ledService.blinkWarn();
        networkService.ButtonHotspot(true, hotspotSsid, hotspotPass);
    } else {
        ledService.lightOnBlue(true);
        networkService.ConnectToWiFi(wiFiConnDetails.ssid,wiFiConnDetails.password);
    }

    ledService.blinkPrimary();

    htmlComponent.setHtmlPageData(wiFiConnDetails.ssid, wiFiConnDetails.password, eventsData, wiFiList);
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
            buttonSettings.saveSettings(postMessage);
        }
        request->send(200, "text/html", "done");
    });

    server.begin();
}

void loop() {
    delay(100);
    if (digitalRead(buttonPin) == 1) {
        Serial.println("[event]");
        ledService.lightOnBlue(true);
        eventService->SendEvents();
        ledService.lightOnBlue(false);
    }
}
