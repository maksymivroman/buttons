//
// Created by rmaks on 27.02.2023.
//

#ifndef EVENT_BUTTON_HTMLCOMPONENTBUILDER_H
#define EVENT_BUTTON_HTMLCOMPONENTBUILDER_H

#include <Arduino.h>
#include "Global/Global.hpp"
#include <ESP8266WiFi.h>
#include "Keystore/Keystore.h"
#include "Global/Version.h"
#include "ButtonState/ButtonState.h"
#include "NetworkService/NetworkService.h"
#include "SettingsService/SettingsService.h"

extern Keystore keystore;
extern Version currentFWVersion;
extern ButtonState buttonState;
extern NetworkService networkService;
extern SettingsService buttonSettings;

class HTMLComponentBuilder {
public:
    String componentById(const String &ref);

    void setHtmlPageData(String ssid, String password, const String *eventsConfig, NETWORKLIST list,
                         EEPROM_SETTINGS config, bool isClient, EEPROM_FLAGS buttonFlags);

private:
    const String *events;
    String networkSsid, networkPassword;
    EEPROM_SETTINGS configuration;
    NETWORKLIST networkList;
    bool isClientMode;
    EEPROM_FLAGS flags;

    String wiFiList();

    String createConfigurationObject(EEPROM_SETTINGS data);
    String getLedConfig(EEPROM_SETTINGS settings);
};


#endif //EVENT_BUTTON_HTMLCOMPONENTBUILDER_H
