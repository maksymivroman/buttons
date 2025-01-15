//
// Created by rmaks on 15.02.2023.
//
#pragma once

#ifndef EVENT_BUTTON_GLOBAL_HPP
#define EVENT_BUTTON_GLOBAL_HPP

#include <Arduino.h>
#include "Logger/Logger.h"
#include <ESP8266WiFi.h>
#include <map>

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

struct LED_MAP {
    RGBCONFIG IDLE_DEFAULT;
    RGBCONFIG IDLE_PRESSED;
    RGBCONFIG LOADING;
    RGBCONFIG WARN;
    RGBCONFIG DONE;
    RGBCONFIG KEYSTORE_UPDATE;
    RGBCONFIG SEND_EVENTS;
    RGBCONFIG EXTERNAL_INTERFACE;
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
    bool overrideLedConfig = false;

    unsigned int wiFiMode = 0;
    unsigned int loggerLevel = 0;
    unsigned int fwVersion = 0;
    unsigned int statisticLevel = 0;

    char ledIdleDefault[8]{"#00FF00"};
    char ledIdlePressed[8]{"#DC5A00"};
    char ledLoading[8]{"#0000FF"};
    char ledWarn[8]{"#FF0000"};
    char ledDone[8]{"#00FF00"};
    char ledKeystoreUpdate[8]{"#FF00FF"};
    char ledSendEvents[8]{"#0000FF"};
    char ledExternalInterface[8]{"#00EBEB"};

    char statisticApi[256]{};
    char wifiSsid[256]{};
    char wifiPass[256]{};
    char hotspotSsid[32]{};
};
const size_t EEPROM_SETTINGS_RESERVED_SIZE = 1024;

struct EEPROM_DYNAMIC {
    bool isPressedState = false;
};
const size_t EEPROM_DYNAMIC_RESERVED_SIZE = 64;

struct EEPROM_FLAGS {
    bool ledRDisabled = false;
    bool ledGDisabled = false;
    bool ledBDisabled = false;
};
const size_t EEPROM_FLAGS_RESERVED_SIZE = 64;

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

enum class ACTIONS {
    IDLE_DEFAULT,
    IDLE_PRESSED,
    LOADING,
    WARN,
    DONE,
    KEYSTORE_UPDATE,
    SEND_EVENTS,
    EXTERNAL_INTERFACE
};

typedef std::map<ACTIONS, RGBCONFIG> LEDConfig;

#endif //EVENT_BUTTON_GLOBAL_HPP
