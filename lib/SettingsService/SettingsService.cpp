//
// Created by rmaks on 21.02.2023.
//

#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <EEPROM.h>
#include "ArduinoJson.h"
#include "SettingsService.h"

WiFiCONFIG SettingsService::getWiFiConnDetails() {
    Serial.println("[SettingsService] -> getWiFiConnDetails");
    WiFiCONFIG eepromWiFiConfig;

    eepromWiFiConfig.ssid = buttonEepromSettings.wifiSsid;
    eepromWiFiConfig.password = buttonEepromSettings.wifiPass;

    return eepromWiFiConfig;
}

String SettingsService::loadEvents() {

    String eventsData;

    bool success = SPIFFS.begin();
    if (success) {
        Serial.println("[SPIFFS] File system mounted with success");
    } else {
        Serial.println("[SPIFFS] Error mounting the dataFile system");
    }

    File dataFile = SPIFFS.open("/post.json", "r");

    if (!dataFile) {
        Serial.println("[SPIFFS] Error opening dataFile for writing. Creating new");
        File fileWrite = SPIFFS.open("/post.json", "w");
        int bytesWritten = fileWrite.print("");
        fileWrite.close();
    } else {
        while (dataFile.available()) {
            eventsData += char(dataFile.read());
        }
        Serial.println("[SPIFFS] dataFile ./post.json:");
        Serial.println(eventsData);
        dataFile.close();
    }

    return eventsData;
}

void SettingsService::saveEvents(String events) {
    Serial.print("[SettingsService] saveEvents: ");
    Serial.println(events);
    File file = SPIFFS.open("/post.json", "w");
    [[maybe_unused]] int bytesWritten = file.print(events);
    file.close();
}

void SettingsService::saveSettings(String settings) {
    StaticJsonDocument<900> jsonDoc;
    deserializeJson(jsonDoc, settings);
    JsonObject data = jsonDoc["inputdata"];
    Serial.print("[SettingsService] events data json: ");
    Serial.println(data);

    WiFiCONFIG wiFiSett;

    const char *wiFiName = data["wifiname"];
    const char *wiFiPassword = data["wifipass"];
    String events = data["eventdata"];

    String config = data["configuration"];
    writeButtonEepromSettings(config);


    Serial.print("[SettingsService] 'eventdata' data json: "); Serial.println(events);
    Serial.print("[SettingsService] 'configuration' data json: "); Serial.println(config);

    wiFiSett.password = wiFiPassword;
    wiFiSett.ssid = wiFiName;

    Serial.print("[SettingsService] data json large: "); Serial.println(data.size());
    Serial.print("[SettingsService] 'eventData' data json: "); Serial.println(events);

    saveEvents(events);
}

void SettingsService::loadButtonEepromSettings() {
    Serial.println("[SettingsService] -> EEPROM Read");
    EEPROM.begin(1024);
    EEPROM.get(0, buttonEepromSettings);
    EEPROM.end();
}

void SettingsService::writeButtonEepromSettings(String &config) {
    EEPROMSETTINGS settings = *new EEPROMSETTINGS;

    StaticJsonDocument<900> jsonSettings;
    deserializeJson(jsonSettings, config);

    String wiFiName = jsonSettings["wifiSsid"] | "";
    String wiFiPassword = jsonSettings["wifiPass"] | "";

    char ssid[256], pass[256];

    wiFiName.toCharArray(ssid,256);
    wiFiPassword.toCharArray(pass,256);

    strcpy(settings.wifiSsid, ssid);
    strcpy(settings.wifiPass, pass);

    settings.serialEnabled = jsonSettings["serialEnabled"].as<bool>() | false;
    settings.clientWebAccess = jsonSettings["clientWebAccess"].as<bool>() | false;
    settings.enableOtaUpdate = jsonSettings["enableOtaUpdate"].as<bool>() | false;
    settings.useDnsName = jsonSettings["useDnsName"].as<bool>() | false;
    settings.useSound = jsonSettings["useSound"].as<bool>() | false;

    Serial.print("[SettingsService] -> EEPROM config size: "); Serial.println(sizeof settings);

    EEPROM.begin(1024);
    EEPROM.put(0, settings);
    EEPROM.end();
}

EEPROMSETTINGS SettingsService::getButtonConfig() {
    return buttonEepromSettings;
}

bool SettingsService::clientWebAccessEnabled() const {
    return buttonEepromSettings.clientWebAccess | false;
}

bool SettingsService::useSoundNotification() const {
    return buttonEepromSettings.useSound | false;
}

