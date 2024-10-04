//
// Created by rmaks on 15.02.2023.
//
#pragma once

#ifndef EVENT_BUTTON_GLOBAL_HPP
#define EVENT_BUTTON_GLOBAL_HPP

#include <Arduino.h>
#include "Logger/Logger.h"
#include <ESP8266WiFi.h>

extern Logger logger;


enum EVENT_TRIGGER {
    DEFAULT_TRIGGER,
    KEYSTORE_UPDATE,
    STATE_PRESSED,
    STATE_RELEASED
};

enum BUTTON_STATE {
    NOT_PRESSED,
    PRESSED
};

enum OPERATION_MODE {
    RUN,
    SETUP
};

enum BUTTON_WIFI_MODE {
    AUTO,
    MODE_11B = WiFiPhyMode::WIFI_PHY_MODE_11B,
    MODE_11G = WiFiPhyMode::WIFI_PHY_MODE_11G,
    MODE_11N = WiFiPhyMode::WIFI_PHY_MODE_11N,
};

struct WiFiCONFIG {
    String ssid;
    String password;
};

struct NETWORKLIST {
    String *arr = nullptr;
    int8_t size = 0;
};

struct RGBCONFIG {
    unsigned char r;
    unsigned char g;
    unsigned char b;
};

struct EEPROM_SETTINGS {
    bool loggerEnabled = false;
    bool statisticEnabled = false;
    bool clientWebAccess = false;
    bool enableOtaUpdate = false;
    bool useDnsName = false;
    bool useSound = false;
    bool useCustomHSsid = false;
    bool useTelegramIntegration = false;
    bool remoteTriggering = false;
    bool keystoreEnabled = false;
    bool sendEventOnKeystoreUpdate = false;
    bool delaySendEvents = false;
    bool remoteStateChange = false;

    bool saveLastState = false;
    bool restoreLastStateOnLoad = false;

    unsigned int wiFiMode = 0;
    unsigned int loggerLevel = 0;
    unsigned int fwVersion = 0;

    unsigned int statisticLevel = 0;
    char statisticApi[256]{};

    char wifiSsid[256]{};
    char wifiPass[256]{};
    char hotspotSsid[32]{};
};

struct EEPROM_DYNAMIC {
    bool isPressedState = false;
};

struct KEYSTORESETTINGS {
    bool enabled;
    bool sendEventsOnUpdate;
    bool delayEvent;
};

struct INTEGRATIONSETTINGS {
    String tToken;
    long long tChanelID;
    String tPrefix;
    String tSuffix;
};

#endif //EVENT_BUTTON_GLOBAL_HPP
