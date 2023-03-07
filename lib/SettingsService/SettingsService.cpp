//
// Created by rmaks on 21.02.2023.
//

#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <EEPROM.h>
#include "ArduinoJson.h"
//#include "html-page.hpp"
//#include "ButtonWebServer.h"
//#include "GlobalConfig.hpp"
#include "SettingsService.h"

WiFiCONFIG SettingsService::loadWiFiSettings() {
    EEPROM.begin(1024);
    Serial.println("[SettingsService] loadWiFiSettings");
    WiFiCONFIG eepromWiFiConfig;

    for (int i = 0; i < 32; ++i)
    {
        if(EEPROM.read(i)!=0){
            eepromWiFiConfig.ssid += char(EEPROM.read(i));
        }
    }

    for (int i = 32; i < 96; ++i)
    {
        if(EEPROM.read(i)!=0){
            eepromWiFiConfig.password += char(EEPROM.read(i));
        }
    }
    Serial.println("[SettingsService] wi-fi data:");
    Serial.println(eepromWiFiConfig.ssid);
    Serial.println(eepromWiFiConfig.password);

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
    }
    else{
        while (dataFile.available()) {
            eventsData+=char(dataFile.read());
        }
        Serial.println("[SPIFFS] dataFile ./post.json:");
        Serial.println(eventsData);
        dataFile.close();
    }

    return eventsData;
}

void SettingsService::saveWiFiSettings(const WiFiCONFIG& settings) {
    if (settings.ssid.length() > 0 && settings.password.length() > 0) {
        EEPROM.begin(1024);

        Serial.println("[SettingsService] clearing eeprom");
        for (int i = 0; i < 1024; ++i) {
            EEPROM.write(i, 0);
        }
        Serial.println("[SettingsService] Network data:");
        Serial.println(settings.ssid);
        Serial.println(settings.password);

        Serial.println("[SettingsService] writing eeprom ssid:");
        for (int i = 0; i < settings.ssid.length(); ++i)
        {
            EEPROM.write(i, settings.ssid[i]);
            Serial.print("[SettingsService] Wrote: ");
            Serial.println(settings.ssid[i]);
        }
        Serial.println("[SettingsService] writing eeprom pass:");
        for (int i = 0; i < settings.password.length(); ++i)
        {
            EEPROM.write(32 + i, settings.password[i]);
            Serial.print("[SettingsService] Wrote: ");
            Serial.println(settings.password[i]);
        }
        EEPROM.commit();
        EEPROM.end();
    };
}

void SettingsService::saveEvents(String events) {
    Serial.print("[SettingsService] saveEvents: ");  Serial.println(events);
    File file = SPIFFS.open("/post.json", "w");
    int bytesWritten = file.print(events);
    file.close();
}

void SettingsService::saveSettings(String settings) {
    StaticJsonDocument<900> jsonDoc;
    deserializeJson(jsonDoc, settings);
    JsonObject data = jsonDoc["inputdata"];
    Serial.print("[SettingsService] events data json: ");  Serial.println(data);

    WiFiCONFIG wiFiSett;

    const char* wiFiName = data["wifiname"];
    const char* wiFiPassword = data["wifipass"];
    String events = data["eventdata"];

    Serial.print("[SettingsService] 'eventdata' data json: ");  Serial.println(events);

    wiFiSett.password = wiFiPassword ;
    wiFiSett.ssid = wiFiName ;

    Serial.print("[SettingsService] data json large: "); Serial.println(data.size());
    Serial.print("[SettingsService] 'eventdata' data json: ");  Serial.println(events);

    saveEvents(events);
    saveWiFiSettings(wiFiSett);
}
