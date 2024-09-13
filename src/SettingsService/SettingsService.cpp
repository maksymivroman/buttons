//
// Created by rmaks on 21.02.2023.
//

#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <EEPROM.h>
#include "ArduinoJson.h"
#include "SettingsService.h"

WiFiCONFIG SettingsService::getWiFiConnDetails() {
    logger.log("[SettingsService] Get WiFi credentials from settings");
    WiFiCONFIG eepromWiFiConfig;

    eepromWiFiConfig.ssid = buttonEepromSettings.wifiSsid;
    eepromWiFiConfig.password = buttonEepromSettings.wifiPass;

    return eepromWiFiConfig;
}

void SettingsService::loadEvents() {
    this->eventsData = dataFromFS("/post.json");
}

const String *SettingsService::events() {
    return &this->eventsData;
}

INTEGRATIONSETTINGS SettingsService::integrationSettings() {
    String data = dataFromFS("/integration.json");
    DynamicJsonDocument jsonDoc(2048);
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
    logger.log("[SettingsService] Save Events");
    logger.logSerial("[SettingsService] saveEvents: ", events);
    File file = SPIFFS.open("/post.json", "w");
    [[maybe_unused]] int bytesWritten = file.print(events);
    file.close();
}

void SettingsService::saveIntegrationSettings(String settings) {
    logger.log("[SettingsService] Save Integration Settings");
    logger.logSerial("[SettingsService] saveIntegrationSettings: ", settings);
    File file = SPIFFS.open("/integration.json", "w");
    [[maybe_unused]] int bytesWritten = file.print(settings);
    file.close();
}


void SettingsService::saveSettings(String settings) {
    DynamicJsonDocument jsonDoc(4096);
    deserializeJson(jsonDoc, settings);
    JsonObject data = jsonDoc["inputdata"];
    logger.log("[SettingsService] Save Settings data (json)");
    logger.logSerial("[SettingsService] events data json: ", data);

    WiFiCONFIG wiFiSett;

    const char *wiFiName = data["wifiname"];
    const char *wiFiPassword = data["wifipass"];
    String events = data["eventdata"];

    String config = data["configuration"];
    writeButtonEepromSettings(config);

    String integrationData = data["integration"];

    logger.logSerial("[SettingsService] 'eventdata' data json: ",events);
    logger.logSerial("[SettingsService] 'configuration' data json: ", config);

    wiFiSett.password = wiFiPassword;
    wiFiSett.ssid = wiFiName;

    logger.log("[SettingsService] data json large: ", data.size());
    logger.logSerial("[SettingsService] 'eventData' data json: ", events);

    saveEvents(events);
    saveIntegrationSettings(integrationData);
}

void SettingsService::loadButtonEepromSettings() {
    logger.log("[SettingsService] Read EEPROM");
    EEPROM.begin(this->eepromSize);
    EEPROM.get(0, buttonEepromSettings);
    EEPROM.end();
}

void SettingsService::writeButtonEepromSettings(String &config) {
    EEPROM_SETTINGS settings = *new EEPROM_SETTINGS;

    DynamicJsonDocument jsonSettings(1024);
    deserializeJson(jsonSettings, config);

    String wiFiName = jsonSettings["wifiSsid"] | "";
    String wiFiPassword = jsonSettings["wifiPass"] | "";
    String hotspotSsid = jsonSettings["hotspotSsid"] | "";

    String statisticApi = jsonSettings["statisticApi"] | "";

    char ssid[256], pass[256], hSsid[32], statApi[256];

    wiFiName.toCharArray(ssid, 256);
    wiFiPassword.toCharArray(pass, 256);
    hotspotSsid.toCharArray(hSsid, 32);

    statisticApi.toCharArray(statApi, 256);

    strcpy(settings.wifiSsid, ssid);
    strcpy(settings.wifiPass, pass);

    strcpy(settings.hotspotSsid, hSsid);

    strcpy(settings.statisticApi, statApi);

    //TODO if button on client mode next settings should not be changed
    settings.clientWebAccess = jsonSettings["clientWebAccess"].as<bool>() | false;
    settings.enableOtaUpdate = jsonSettings["enableOtaUpdate"].as<bool>() | false;

    settings.loggerEnabled = jsonSettings["loggerEnabled"].as<bool>() | false;
    settings.statisticEnabled = jsonSettings["statisticEnabled"].as<bool>() | false;
    settings.useDnsName = jsonSettings["useDnsName"].as<bool>() | false;
    settings.useSound = jsonSettings["useSound"].as<bool>() | false;
    settings.useTelegramIntegration = jsonSettings["useTelegramIntegration"].as<bool>() | false;
    settings.remoteTriggering = jsonSettings["remoteTriggering"].as<bool>() | false;
    settings.useCustomHSsid = jsonSettings["customHSsid"].as<bool>() | false;
    settings.loggerLevel = jsonSettings["loggerLevel"].as<unsigned int>() | 0;
    settings.statisticLevel = jsonSettings["statisticLevel"].as<unsigned int>() | 0;
    settings.remoteStateChange = jsonSettings["remoteStateChange"].as<bool>() | false;
    settings.saveLastState = jsonSettings["saveLastState"].as<bool>() | false;

    settings.keystoreEnabled = jsonSettings["keystoreEnabled"].as<bool>() | false;
    settings.sendEventOnKeystoreUpdate = jsonSettings["sendEventOnKeystoreUpdate"].as<bool>() | false;
    settings.delaySendEvents = jsonSettings["delaySendEvents"].as<bool>() | false;

    settings.fwVersion = this->buttonEepromSettings.fwVersion;

    logger.log("[SettingsService] -> EEPROM config size: ", sizeof settings);

    this->writeToEEPROM(settings);
}

EEPROM_SETTINGS SettingsService::getButtonConfig() {
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

bool SettingsService::otaUpdateOnClientMode() const {
    return buttonEepromSettings.enableOtaUpdate | false;
}

bool SettingsService::remoteButtonTriggering() const {
    return buttonEepromSettings.remoteTriggering | false;
}

bool SettingsService::loggerEnabled() const {
    return buttonEepromSettings.loggerEnabled | false;
}

bool SettingsService::statisticEnabled() const {
    return buttonEepromSettings.statisticEnabled | false;
}

bool SettingsService::remoteStateChangeEnabled() const {
    return buttonEepromSettings.remoteStateChange | false;
}

bool SettingsService::saveLastState() const {
    return buttonEepromSettings.saveLastState | false;
}

LoggerLevel SettingsService::loggerLevel() const {
    return static_cast<LoggerLevel>(buttonEepromSettings.loggerLevel);
}

unsigned int SettingsService::statisticLevel() const {
    return buttonEepromSettings.statisticLevel;
}

String SettingsService::statisticApi() const {
    return buttonEepromSettings.statisticApi;
}

void SettingsService::clearEeprom() {
    logger.log("[SettingsService] Start clear EEPROM. Size: ", this->totalEepromSize(), " ...");
    EEPROM.begin(this->totalEepromSize());
    for (int i = 0; i < this->totalEepromSize(); ++i) {
        EEPROM.write(i, 0);
    }
    EEPROM.commit();
    EEPROM.end();
    logger.log("Done!");
}

char *SettingsService::customHotspotSsid() {
    String espDefaultName = "eButton-";
    const String mac = WiFi.macAddress();
    espDefaultName += mac.substring(mac.length() - 6, mac.length());
    espDefaultName.replace(':', 'x');

    const bool useCustomSsid = buttonEepromSettings.hotspotSsid[0] != '\0';
    if (useCustomSsid && buttonEepromSettings.useCustomHSsid) {
        return buttonEepromSettings.hotspotSsid;
    }
    char *name = new char[espDefaultName.length() + 1];
    strcpy(name, espDefaultName.c_str());

    return const_cast<char *>(name);
}

String SettingsService::deviceID() const{
    String espDefaultName = "eButton-";
    const String mac = WiFi.macAddress();
    espDefaultName += mac.substring(mac.length() - 6, mac.length());
    espDefaultName.replace(':', 'x');
    return espDefaultName;
}

String SettingsService::dataFromFS(const String &fileName) {
    String data;
    const char *file = fileName.c_str();

    bool success = SPIFFS.begin();
    if (success) {
        logger.log("[SettingsService][SPIFFS] File system mounted with success");

    } else {
        logger.log("[SettingsService][SPIFFS] Error mounting the dataFile system");
    }

    File dataFile = SPIFFS.open(file, "r");

    if (!dataFile) {
        logger.log("[SettingsService][SPIFFS] Error opening dataFile for writing. Creating new");
        File fileWrite = SPIFFS.open(file, "w");
        [[maybe_unused]] int bytesWritten = fileWrite.print("{}");
        fileWrite.close();
        return "{}";
    } else {
        while (dataFile.available()) {
            data += char(dataFile.read());
        }
        logger.log("[SettingsService][SPIFFS] File name: ", file);
        logger.logSerial("[SettingsService][SPIFFS] File data: ", data);
        dataFile.close();
        return data;
    }
}

void SettingsService::formatFS() {
    logger.log("[SettingsService][SPIFFS] Prepare to Format FS");
    bool success = SPIFFS.begin();
    if (success) {
        logger.log("[SettingsService][SPIFFS] File system mounted with success");

        Dir root = SPIFFS.openDir("/");
        logger.log("[SettingsService][SPIFFS] Try to open...");

        while (root.next()) {
            logger.log("[SettingsService][SPIFFS] File: ", root.fileName());
        }

        logger.log("[SettingsService][SPIFFS] Formatting FS...");
        bool formatted = SPIFFS.format();
        formatted ? logger.log("DONE") : logger.log("FAILED!");
        SPIFFS.end();
    } else {
        logger.log("[SettingsService][SPIFFS] Error mounting the dataFile system");
    }
    logger.log("[SettingsService][SPIFFS] Exit Format FS");
}

KEYSTORESETTINGS SettingsService::keystoreSettings() const {
    KEYSTORESETTINGS settings = {
            buttonEepromSettings.keystoreEnabled,
            buttonEepromSettings.sendEventOnKeystoreUpdate,
            buttonEepromSettings.delaySendEvents
    };
    return settings;
}

unsigned int SettingsService::fwVersion() const {
    return buttonEepromSettings.fwVersion;
}

void SettingsService::writeToEEPROM(EEPROM_SETTINGS settings) {
    logger.log("[SettingsService] -> Write to EEPROM ", sizeof settings, " bytes...");
    EEPROM.begin(this->eepromSize);
    EEPROM.put(0, settings);
    EEPROM.end();
    logger.log("[SettingsService] -> Write to EEPROM. DONE");
}

void SettingsService::updateDynamicEEPROM(EEPROM_DYNAMIC dynamicProps) {
    logger.log("[SettingsService] -> Write to EEPROM_DYNAMIC ", sizeof dynamicProps, " bytes...");
    EEPROM.begin(this->dynamicEepromSize + this->dynamicEepromStartOffset());
    EEPROM.put(this->dynamicEepromStartOffset(), dynamicProps);
    EEPROM.end();
    logger.log("[SettingsService] -> Write to EEPROM_DYNAMIC. DONE");
}

void SettingsService::handleVersionChange(unsigned int currentFWVersion, bool requireEEPROMFormat = false) {
    const bool versionChanged = buttonEepromSettings.fwVersion != currentFWVersion;

    if (versionChanged && requireEEPROMFormat) {
        EEPROM_SETTINGS defaultEEPROMConfig;
        defaultEEPROMConfig.fwVersion = currentFWVersion;
        this->clearEeprom();
        this->writeToEEPROM(defaultEEPROMConfig);
        this->buttonEepromSettings = defaultEEPROMConfig;
    }else {
        this->buttonEepromSettings.fwVersion = currentFWVersion;
        this->writeToEEPROM(buttonEepromSettings);
    }
}

String SettingsService::macAddress() const {
    return WiFi.macAddress();
}

String SettingsService::localIPAddress() const {
    switch (WiFi.getMode()) {
        case WIFI_STA:
            return WiFi.localIP().toString();
        case WIFI_AP_STA:
            return WiFi.softAPIP().toString();
        default:
            return "unknown";
    }
}

size_t SettingsService::dynamicEepromStartOffset() const {
    return static_cast<const int>(this->eepromSize) + 1;
}

void SettingsService::loadButtonDynamicProps() {
    logger.log("[SettingsService] Read Dynamic EEPROM");
    EEPROM.begin(this->dynamicEepromSize + this->dynamicEepromStartOffset());
    EEPROM.get(this->dynamicEepromStartOffset(), this->buttonDynamicEeprom);
    EEPROM.end();
}

bool SettingsService::getLastStatePressed() {
    logger.log("[SettingsService] Read isPressedState: ", buttonDynamicEeprom.isPressedState);
    return buttonDynamicEeprom.isPressedState;
}

size_t SettingsService::totalEepromSize() const {
    return this->dynamicEepromStartOffset() + this->dynamicEepromSize;
}

void SettingsService::setLastState(int state) {
    logger.log("[SettingsService] Set button state to ", state);
    EEPROM_DYNAMIC props = *new EEPROM_DYNAMIC;
    props.isPressedState = state;
    this->updateDynamicEEPROM(props);
    buttonDynamicEeprom.isPressedState = state;
}
