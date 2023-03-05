#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ESP8266HTTPClient.h>
#include "ArduinoJson.h"
#include <EEPROM.h>
#include <DNSServer.h>
//New
#include "LEDService.h"
#include "html-page.hpp"
#include "ButtonWebServer.h"
#include "GlobalConfig.hpp"
#include "SettingsService.h"
#include "NetworkService.h"
#include "HTMLComponentBuilder.h"

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

String components(const String &ref) {
    return htmlComponent.componentById(ref);
}

void setup() {
    Serial.begin(115200);
    ledService.pinConfig(redPin, greenPin, bluePin);
    pinMode(buttonPin, INPUT);
    WiFiCONFIG wiFiConnDetails = buttonSettings.loadWiFiSettings();
    String eventsData = buttonSettings.loadEvents();

    NETWORKLIST wiFiList;
    wiFiList = networkService.WiFiList();

    if (digitalRead(buttonPin) == 0) {
        ledService.blinkWarn();
        networkService.ButtonHotspot(true, hotspotSsid, hotspotPass);
    }

    ledService.blinkPrimary();

    htmlComponent.setHtmlPageData(wiFiConnDetails.ssid, wiFiConnDetails.password, eventsData, wiFiList);

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send_P(200, "text/html", index_html, components);
    });


    server.begin();
}

void loop() {

}