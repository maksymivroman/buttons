//
// Created by rmaks on 26.02.2023.
//

#ifndef EVENT_BUTTON_NETWORKSERVICE_H
#define EVENT_BUTTON_NETWORKSERVICE_H

#include <Arduino.h>
#include "GlobalConfig.hpp"

class NetworkService {

public:
    void ButtonHotspot(bool isOn, const char* ssid, const char* pass);
    void ConnectToWiFi(const String& ssid, const String& pass);
    NETWORKLIST WiFiList();
    bool isConnectedToWiFi();
    bool isAPMode();
};


#endif //EVENT_BUTTON_NETWORKSERVICE_H
