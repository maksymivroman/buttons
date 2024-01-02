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
    EEPROMSETTINGS getButtonConfig();

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
    unsigned int fwVersion() const;
    LoggerLevel loggerLevel() const;
    char *customHotspotSsid();
    KEYSTORESETTINGS keystoreSettings() const;

    void handleVersionChange(unsigned int currentFWVersion, bool requireEEPROMFormat);

private:
    void writeButtonEepromSettings(String& config);
    void saveIntegrationSettings(String settings);
    void writeToEEPROM(EEPROMSETTINGS settings);

    String dataFromFS(const String& fileName);
    String eventsData;

    EEPROMSETTINGS buttonEepromSettings;
};


#endif //EVENT_BUTTON_SETTINGSSERVICE_H
