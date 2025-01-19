//
// Created by rmaks on 21.02.2023.
//

#ifndef EVENT_BUTTON_SETTINGSSERVICE_H
#define EVENT_BUTTON_SETTINGSSERVICE_H

#include <Arduino.h>
#include "Global/Global.hpp"

class SettingsService {
public:
    WiFiCONFIG getWiFiConnDetails();
    const String *events();
    EEPROM_SETTINGS getButtonConfig();
    EEPROM_FLAGS buttonFlags() const;

    void saveEvents(String events);
    void saveSettings(String &settings);
    void loadButtonEepromSettings();
    void loadEvents();
    void clearEeprom();
    void formatFS();

    void updateFlagsEEPROM(EEPROM_FLAGS flags);

    bool clientWebAccessEnabled() const;
    bool useSoundNotification() const;
    bool otaUpdateOnClientMode() const;
    bool remoteButtonTriggering() const;
    bool loggerEnabled() const;
    bool statisticEnabled() const;
    bool remoteStateChangeEnabled() const;
    bool overrideLedConfig() const;
    bool saveLastState() const;
    bool restoreLastStateOnLoad() const;

    String statisticApi() const;
    unsigned int fwVersion() const;
    LoggerLevel loggerLevel() const;
    BUTTON_WIFI_MODE wiFiMode() const;
    unsigned int statisticLevel() const;
    char *customHotspotSsid();
    String deviceID() const;
    KEYSTORESETTINGS keystoreSettings() const;

    void loadButtonDynamicProps();
    void loadButtonFlags();
    bool getLastStatePressed();
    void setLastState(int state);

    void handleVersionChange(unsigned int currentFWVersion, bool requireEEPROMFormat);

    String macAddress() const;
    String localIPAddress() const;

    LED_MAP ledMap();

private:
    void writeButtonEepromSettings(String& config);
    void writeToEEPROM(EEPROM_SETTINGS settings);

    void updateDynamicEEPROM(EEPROM_DYNAMIC dynamicProps);

    String dataFromFS(const String& fileName);
    String eventsData;

    EEPROM_SETTINGS buttonEepromSettings;
    EEPROM_DYNAMIC buttonDynamicEeprom;
    EEPROM_FLAGS buttonFlagsEeprom;

    size_t dynamicEepromStartOffset() const;
    size_t flagsEepromStartOffset() const;
    size_t totalEepromSize() const;

    RGBCONFIG hexToRGB(const char hex[]);
};


#endif //EVENT_BUTTON_SETTINGSSERVICE_H
