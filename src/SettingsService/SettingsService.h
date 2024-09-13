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
    INTEGRATIONSETTINGS integrationSettings();
    EEPROM_SETTINGS getButtonConfig();

    void saveEvents(String events);
    void saveSettings(String settings);
    void loadButtonEepromSettings();
    void loadEvents();
    void clearEeprom();
    void formatFS();
    bool clientWebAccessEnabled() const;
    bool useSoundNotification() const;
    bool useTelegramIntegration() const;
    bool otaUpdateOnClientMode() const;
    bool remoteButtonTriggering() const;
    bool loggerEnabled() const;
    bool statisticEnabled() const;
    bool remoteStateChangeEnabled() const;

    bool saveLastState() const;
    bool restoreLastStateOnLoad() const;

    String statisticApi() const;
    unsigned int fwVersion() const;
    LoggerLevel loggerLevel() const;
    unsigned int statisticLevel() const;
    char *customHotspotSsid();
    String deviceID() const;
    KEYSTORESETTINGS keystoreSettings() const;

    void loadButtonDynamicProps();
    bool getLastStatePressed();
    void setLastState(int state);

    void handleVersionChange(unsigned int currentFWVersion, bool requireEEPROMFormat);

    String macAddress() const;
    String localIPAddress() const;

private:
    void writeButtonEepromSettings(String& config);
    void saveIntegrationSettings(String settings);
    void writeToEEPROM(EEPROM_SETTINGS settings);

    void updateDynamicEEPROM(EEPROM_DYNAMIC dynamicProps);

    String dataFromFS(const String& fileName);
    String eventsData;

    EEPROM_SETTINGS buttonEepromSettings;
    EEPROM_DYNAMIC buttonDynamicEeprom;

    const size_t eepromSize = 1024;
    const size_t dynamicEepromSize = 64;

    size_t dynamicEepromStartOffset() const;
    size_t totalEepromSize() const;
};


#endif //EVENT_BUTTON_SETTINGSSERVICE_H
