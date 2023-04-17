//
// Created by rmaks on 15.02.2023.
//
#pragma once

#ifndef EVENT_BUTTON_GLOBALCONFIG_HPP
#define EVENT_BUTTON_GLOBALCONFIG_HPP

#include <Arduino.h>

struct WiFiCONFIG {
    String ssid;
    String password;
};

struct NETWORKLIST {
    String* arr = nullptr;
    int8_t size = 0;
};

struct RGBCONFIG {
    int r;
    int g;
    int b;
};

struct EEPROMSETTINGS {
    bool serialEnabled = false;
    bool clientWebAccess = false;
    bool enableOtaUpdate = false;
    bool useDnsName = false;
    bool useSound = false;
    bool useCustomHSsid = false;
    bool reservedB = false;
    bool reservedC = false;
    unsigned int fwVersion = 0;
    char wifiSsid[256]{};
    char wifiPass[256]{};
    char hotspotSsid[32]{};
};

#endif //EVENT_BUTTON_GLOBALCONFIG_HPP
