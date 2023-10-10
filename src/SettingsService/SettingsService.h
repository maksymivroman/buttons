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
    String loadEvents();
    INTEGRATIONSETTINGS integrationSettings();
    EEPROMSETTINGS getButtonConfig();

    void saveEvents(String events);
    void saveSettings(String settings);
    void loadButtonEepromSettings();
    void clearEeprom();
    bool clientWebAccessEnabled() const;
    bool useSoundNotification() const;
    bool useTelegramIntegration() const;
    bool otaUpdateOnClientMode() const;
    bool remoteButtonTriggering() const;
    char * customHotspotSsid();

private:
    void writeButtonEepromSettings(String& config);
    void saveIntegrationSettings(String settings);

    String dataFromFS(const String& fileName);

    EEPROMSETTINGS buttonEepromSettings;
};


#endif //EVENT_BUTTON_SETTINGSSERVICE_H
