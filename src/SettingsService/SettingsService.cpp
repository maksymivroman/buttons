//
// Created by rmaks on 21.02.2023.
//

#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <EEPROM.h>
#include "ArduinoJson.h"
#include "SettingsService.h"

WiFiCONFIG SettingsService::getWiFiConnDetails() {
    WiFiCONFIG eepromWiFiConfig{buttonEepromSettings.wifiSsid, buttonEepromSettings.wifiPass};
    return eepromWiFiConfig;
}

void SettingsService::loadEvents() {
    this->eventsData = dataFromFS(EVENTS_FILE_NAME);
}

const String *SettingsService::events() {
    return &this->eventsData;
}

void SettingsService::saveEvents(String events) {
    logger.log("[SettingsService] Save Events");
    logger.logSerial("[SettingsService] saveEvents: ", events);
    File file = SPIFFS.open(EVENTS_FILE_NAME, "w");
    [[maybe_unused]] int bytesWritten = file.print(events);
    file.close();
    this->eventsData = events;
}

void SettingsService::saveSettings(String &settings) {
    DynamicJsonDocument jsonDoc(4096);
    deserializeJson(jsonDoc, settings);
    logger.log("[SettingsService] Save Settings data (json). Size: ", sizeof(jsonDoc));

    String config = jsonDoc["configuration"];

    writeButtonEepromSettings(config);

    logger.logSerial("[SettingsService] 'configuration' data json: ", config);
}

void SettingsService::loadButtonEepromSettings() {
    logger.log("[SettingsService] Read EEPROM");
    EEPROM.begin(EEPROM_SETTINGS_RESERVED_SIZE);
    EEPROM.get(0, buttonEepromSettings);
    EEPROM.end();
}

void SettingsService::loadButtonDynamicProps() {
    logger.log("[SettingsService] Read Dynamic EEPROM");
    EEPROM.begin(this->totalEepromSize());
    EEPROM.get(this->dynamicEepromStartOffset(), this->buttonDynamicEeprom);
    EEPROM.end();
}

void SettingsService::loadButtonFlags() {
    logger.log("[SettingsService] Read Dynamic EEPROM");
    EEPROM.begin(this->totalEepromSize());
    EEPROM.get(this->flagsEepromStartOffset(), this->buttonFlagsEeprom);
    EEPROM.end();
}

void SettingsService::writeButtonEepromSettings(String &config) {
    EEPROM_SETTINGS settings = *new EEPROM_SETTINGS;

    DynamicJsonDocument jsonSettings(1536);
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
    settings.remoteTriggering = jsonSettings["remoteTriggering"].as<bool>() | false;
    settings.useCustomHSsid = jsonSettings["customHSsid"].as<bool>() | false;
    settings.loggerLevel = jsonSettings["loggerLevel"].as<unsigned int>() | 0;
    settings.wiFiMode = jsonSettings["wiFiMode"].as<unsigned int>() | 0;
    settings.statisticLevel = jsonSettings["statisticLevel"].as<unsigned int>() | 0;
    settings.remoteStateChange = jsonSettings["remoteStateChange"].as<bool>() | false;
    settings.saveLastState = jsonSettings["saveLastState"].as<bool>() | false;
    settings.restoreLastStateOnLoad = jsonSettings["restoreLastStateOnLoad"].as<bool>() | false;
    settings.keystoreEnabled = jsonSettings["keystoreEnabled"].as<bool>() | false;
    settings.sendEventOnKeystoreUpdate = jsonSettings["sendEventOnKeystoreUpdate"].as<bool>() | false;
    settings.delaySendEvents = jsonSettings["delaySendEvents"].as<bool>() | false;
    settings.overrideLedConfig = jsonSettings["overrideLedConfig"].as<bool>() | false;
    settings.serialEvents = jsonSettings["serialEvents"].as<bool>() | false;
    settings.customServer = jsonSettings["customServer"].as<bool>() | false;
    settings.timezone = static_cast<TimeZoneId>(jsonSettings["timezone"].as<unsigned int>() | 0);
    strncpy(settings.ledIdleDefault, String(jsonSettings["ledConfig"]["ledIdleDefault"]).c_str(), 7);
    strncpy(settings.ledIdlePressed, String(jsonSettings["ledConfig"]["ledIdlePressed"]).c_str(), 7);
    strncpy(settings.ledLoading, String(jsonSettings["ledConfig"]["ledLoading"]).c_str(), 7);
    strncpy(settings.ledWarn, String(jsonSettings["ledConfig"]["ledWarn"]).c_str(), 7);
    strncpy(settings.ledDone, String(jsonSettings["ledConfig"]["ledDone"]).c_str(), 7);
    strncpy(settings.ledKeystoreUpdate, String(jsonSettings["ledConfig"]["ledKeystoreUpdate"]).c_str(), 7);
    strncpy(settings.ledSendEvents, String(jsonSettings["ledConfig"]["ledSendEvents"]).c_str(), 7);
    strncpy(settings.ledExternalInterface, String(jsonSettings["ledConfig"]["ledExternalInterface"]).c_str(), 7);

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

bool SettingsService::restoreLastStateOnLoad() const {
    return buttonEepromSettings.restoreLastStateOnLoad | false;
}

bool SettingsService::overrideLedConfig() const {
    return buttonEepromSettings.overrideLedConfig | false;
}

bool SettingsService::serialEvents() const {
    return buttonEepromSettings.serialEvents | false;
}

bool SettingsService::customServer() const {
    return buttonEepromSettings.customServer | false;
}

LoggerLevel SettingsService::loggerLevel() const {
    return static_cast<LoggerLevel>(buttonEepromSettings.loggerLevel);
}

BUTTON_WIFI_MODE SettingsService::wiFiMode() const {
    return static_cast<BUTTON_WIFI_MODE>(buttonEepromSettings.wiFiMode);
}

unsigned int SettingsService::statisticLevel() const {
    return buttonEepromSettings.statisticLevel;
}

String SettingsService::statisticApi() const {
    return buttonEepromSettings.statisticApi;
}

EEPROM_FLAGS SettingsService::buttonFlags() const {
    return this->buttonFlagsEeprom;
}

TimeZoneId SettingsService::timeZoneId() const {
    return buttonEepromSettings.timezone;
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
    EEPROM.begin(this->totalEepromSize());
    EEPROM.put(0, settings);
    EEPROM.end();
    logger.log("[SettingsService] -> Write to EEPROM. DONE");
}

void SettingsService::updateDynamicEEPROM(EEPROM_DYNAMIC dynamicProps) {
    logger.log("[SettingsService] -> Write to EEPROM_DYNAMIC ", sizeof dynamicProps, " bytes...");
    EEPROM.begin(this->totalEepromSize());
    EEPROM.put(this->dynamicEepromStartOffset(), dynamicProps);
    EEPROM.end();
    logger.log("[SettingsService] -> Write to EEPROM_DYNAMIC. DONE");
}

void SettingsService::updateFlagsEEPROM(EEPROM_FLAGS flags) {
    logger.log("[SettingsService] -> Write to EEPROM_FLAGS ", sizeof flags, " bytes...");
    EEPROM.begin(this->totalEepromSize());
    EEPROM.put(this->flagsEepromStartOffset(), flags);
    EEPROM.end();
    logger.log("[SettingsService] -> Write to EEPROM_FLAGS. DONE");
}

bool SettingsService::handleVersionChange(unsigned int currentFWVersion, bool requireEEPROMFormat = false) {
    const bool versionChanged = buttonEepromSettings.fwVersion != currentFWVersion;

    if (versionChanged && requireEEPROMFormat) {
        EEPROM_SETTINGS defaultEEPROMConfig;
        defaultEEPROMConfig.fwVersion = currentFWVersion;
        this->clearEeprom();
        this->writeToEEPROM(defaultEEPROMConfig);
        this->buttonEepromSettings = defaultEEPROMConfig;
        return true;
    }else {
        this->buttonEepromSettings.fwVersion = currentFWVersion;
        this->writeToEEPROM(buttonEepromSettings);
        return false;
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
    return EEPROM_SETTINGS_RESERVED_SIZE;
}

size_t SettingsService::flagsEepromStartOffset() const {
    return EEPROM_SETTINGS_RESERVED_SIZE + EEPROM_DYNAMIC_RESERVED_SIZE;
}

bool SettingsService::getLastStatePressed() {
    logger.log("[SettingsService] Read isPressedState: ", buttonDynamicEeprom.isPressedState);
    return buttonDynamicEeprom.isPressedState;
}

size_t SettingsService::totalEepromSize() const {
    return EEPROM_SETTINGS_RESERVED_SIZE + EEPROM_DYNAMIC_RESERVED_SIZE + EEPROM_FLAGS_RESERVED_SIZE;
}

void SettingsService::setLastState(int state) {
    logger.log("[SettingsService] Set button state to ", state);
    EEPROM_DYNAMIC props = *new EEPROM_DYNAMIC;
    props.isPressedState = state;
    this->updateDynamicEEPROM(props);
    buttonDynamicEeprom.isPressedState = state;
}

RGBCONFIG SettingsService::hexToRGB(const char *hex) {
    const char *cleanHex = (hex[0] == '#') ? &hex[1] : hex;
    unsigned int hexValue = strtol(cleanHex, nullptr, 16);
    return {
            .r = static_cast<unsigned char>((hexValue >> 16) & 0xFF),
            .g = static_cast<unsigned char>((hexValue >> 8) & 0xFF),
            .b = static_cast<unsigned char>(hexValue & 0xFF)
    };
}

LED_MAP SettingsService::ledMap() {
    return {
            {hexToRGB(buttonEepromSettings.ledIdleDefault)},
            {hexToRGB(buttonEepromSettings.ledIdlePressed)},
            {hexToRGB(buttonEepromSettings.ledLoading)},
            {hexToRGB(buttonEepromSettings.ledWarn)},
            {hexToRGB(buttonEepromSettings.ledDone)},
            {hexToRGB(buttonEepromSettings.ledKeystoreUpdate)},
            {hexToRGB(buttonEepromSettings.ledSendEvents)},
            {hexToRGB(buttonEepromSettings.ledExternalInterface)}
    };
}

const SERVER_CONFIG SettingsService::serverConfig() {
    if (!this->serverConfigLoaded && SPIFFS.exists(SERVER_CONF_PATH)) {
        logger.log("[SettingsService] Loading custom server config");
        File confFile = SPIFFS.open(SERVER_CONF_PATH, "r");
        StaticJsonDocument<128> doc;
        deserializeJson(doc, confFile);
        this->currentServerPath = doc["path"].as<String>();
        confFile.close();
    }
    logger.log("[SettingsService] Custom server config loaded. Server path: ", this->currentServerPath);
    this->serverConfigLoaded = true;
    return { this->currentServerPath };
}

void SettingsService::saveServerConfig(String path) {
    logger.log("[SettingsService] Saving custom server config: ", path);
    File confFile = SPIFFS.open(SERVER_CONF_PATH, "w");
    if (confFile) {
        StaticJsonDocument<256> doc;
        doc["path"] = path;
        serializeJson(doc, confFile);
        confFile.close();
        this->currentServerPath = path;
    }
}
