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
    return dataFromFS("/post.json");
}

INTEGRATIONSETTINGS SettingsService::integrationSettings() {
    String data = dataFromFS("/integration.json");
    StaticJsonDocument<900> jsonDoc;
    deserializeJson(jsonDoc, data);
    String tToken = jsonDoc["tToken"] | "";
    long long tChanelID = jsonDoc["tChanelID"];
    String tPrefix = jsonDoc["tPrefix"] | "";
    String tSuffix = jsonDoc["tSuffix"] | "";

    INTEGRATIONSETTINGS settings;
    settings.tToken = tToken;
    settings.tChanelID = tChanelID;
    settings.tPrefix = tPrefix;
    settings.tSuffix = tSuffix;

    return settings;
}

void SettingsService::saveEvents(String events) {
    Serial.print("[SettingsService] saveEvents: ");
    Serial.println(events);
    File file = SPIFFS.open("/post.json", "w");
    [[maybe_unused]] int bytesWritten = file.print(events);
    file.close();
}

void SettingsService::saveIntegrationSettings(String settings) {
    Serial.print("[SettingsService] saveIntegrationSettings: ");
    Serial.println(settings);
    File file = SPIFFS.open("/integration.json", "w");
    [[maybe_unused]] int bytesWritten = file.print(settings);
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

    String integrationData = data["integration"];

    Serial.print("[SettingsService] 'eventdata' data json: "); Serial.println(events);
    Serial.print("[SettingsService] 'configuration' data json: "); Serial.println(config);

    wiFiSett.password = wiFiPassword;
    wiFiSett.ssid = wiFiName;

    Serial.print("[SettingsService] data json large: "); Serial.println(data.size());
    Serial.print("[SettingsService] 'eventData' data json: "); Serial.println(events);

    saveEvents(events);
    saveIntegrationSettings(integrationData);
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
    String hotspotSsid = jsonSettings["hotspotSsid"] | "";

    char ssid[256], pass[256], hSsid[32];

    wiFiName.toCharArray(ssid,256);
    wiFiPassword.toCharArray(pass,256);
    hotspotSsid.toCharArray(hSsid, 32);

    strcpy(settings.wifiSsid, ssid);
    strcpy(settings.wifiPass, pass);

    strcpy(settings.hotspotSsid, hSsid);

    settings.serialEnabled = jsonSettings["serialEnabled"].as<bool>() | false;
    settings.clientWebAccess = jsonSettings["clientWebAccess"].as<bool>() | false;
    settings.enableOtaUpdate = jsonSettings["enableOtaUpdate"].as<bool>() | false;
    settings.useDnsName = jsonSettings["useDnsName"].as<bool>() | false;
    settings.useSound = jsonSettings["useSound"].as<bool>() | false;
    settings.useTelegramIntegration = jsonSettings["useTelegramIntegration"].as<bool>() | false;
    settings.useCustomHSsid = jsonSettings["customHSsid"].as<bool>() | false;

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

bool SettingsService::useTelegramIntegration() const {
    return buttonEepromSettings.useTelegramIntegration | false;
}

void SettingsService::clearEeprom() {
    Serial.print("[SettingsService] -> Start clear EEPROM [1024] ...");
    EEPROM.begin(1024);
    for (int i = 0; i < 1024; ++i) {
        EEPROM.write(i, 0);
    }
    EEPROM.commit();
    EEPROM.end();
    Serial.println("Done!");
}

char * SettingsService::customHotspotSsid() {
    String espDefaultName = "eButton-";
    const String mac = WiFi.macAddress();
    espDefaultName += mac.substring(mac.length() - 6, mac.length());
    espDefaultName.replace(':','x');

    const bool useCustomSsid = buttonEepromSettings.hotspotSsid[0] != '\0';
    if (useCustomSsid && buttonEepromSettings.useCustomHSsid ) {
        return buttonEepromSettings.hotspotSsid;
    }
    char *name = new char[espDefaultName.length() + 1];
    strcpy(name, espDefaultName.c_str());

    return const_cast<char *>(name);
}

String SettingsService::dataFromFS(const String& fileName) {
    String data;
    const char *file = fileName.c_str();

    bool success = SPIFFS.begin();
    if (success) {
        Serial.println("[SPIFFS] File system mounted with success");
    } else {
        Serial.println("[SPIFFS] Error mounting the dataFile system");
    }

    File dataFile = SPIFFS.open(file, "r");

    if (!dataFile) {
        Serial.println("[SPIFFS] Error opening dataFile for writing. Creating new");
        File fileWrite = SPIFFS.open(file, "w");
        int bytesWritten = fileWrite.print("");
        fileWrite.close();
    } else {
        while (dataFile.available()) {
            data += char(dataFile.read());
        }
        Serial.print("[SPIFFS] File name/data:"); Serial.println(file);
        Serial.println(data);
        dataFile.close();
    }

    return data;
}
