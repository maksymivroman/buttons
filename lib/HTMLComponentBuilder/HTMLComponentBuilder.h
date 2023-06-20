//
// Created by rmaks on 27.02.2023.
//

#ifndef EVENT_BUTTON_HTMLCOMPONENTBUILDER_H
#define EVENT_BUTTON_HTMLCOMPONENTBUILDER_H

#include <Arduino.h>
#include "GlobalConfig.hpp"
#include <ESP8266WiFi.h>

class HTMLComponentBuilder {
public:
    String componentById(const String &ref);

    void setHtmlPageData(String ssid, String password, String eventsConfig, NETWORKLIST list, EEPROMSETTINGS config, INTEGRATIONSETTINGS integrationData, bool isClient);

private:
    String events, networkSsid, networkPassword;
    EEPROMSETTINGS configuration;
    NETWORKLIST networkList;
    INTEGRATIONSETTINGS integrationSettings;
    bool isClientMode;

    String wiFiList();

    String createConfigurationObject(EEPROMSETTINGS data);
    String createIntegrationDataObject(INTEGRATIONSETTINGS data);
};


#endif //EVENT_BUTTON_HTMLCOMPONENTBUILDER_H
