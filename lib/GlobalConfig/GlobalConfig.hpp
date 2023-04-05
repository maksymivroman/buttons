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

#endif //EVENT_BUTTON_GLOBALCONFIG_HPP
