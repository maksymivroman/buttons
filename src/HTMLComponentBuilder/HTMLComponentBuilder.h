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

extern Keystore keystore;
extern Version currentFWVersion;
extern ButtonState buttonState;
extern NetworkService networkService;

class HTMLComponentBuilder {
public:
    String componentById(const String &ref);

    void setHtmlPageData(String ssid, String password, const String *eventsConfig, NETWORKLIST list, EEPROM_SETTINGS config, INTEGRATIONSETTINGS integrationData, bool isClient);

private:
    const String *events;
    String networkSsid, networkPassword;
    EEPROM_SETTINGS configuration;
    NETWORKLIST networkList;
    INTEGRATIONSETTINGS integrationSettings;
    bool isClientMode;

    String wiFiList();

    String createConfigurationObject(EEPROM_SETTINGS data);
    String createIntegrationDataObject(INTEGRATIONSETTINGS data);
    String getLedConfig(EEPROM_SETTINGS settings);
};


#endif //EVENT_BUTTON_HTMLCOMPONENTBUILDER_H
