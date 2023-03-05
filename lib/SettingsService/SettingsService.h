//
// Created by rmaks on 21.02.2023.
//

#ifndef EVENT_BUTTON_SETTINGSSERVICE_H
#define EVENT_BUTTON_SETTINGSSERVICE_H

#include <Arduino.h>
#include "GlobalConfig.hpp"

class SettingsService {
public:
    WiFiCONFIG loadWiFiSettings();
    String loadEvents();
    void saveWiFiSettings(WiFiCONFIG settings);
    void saveEvents(String events);
};


#endif //EVENT_BUTTON_SETTINGSSERVICE_H
