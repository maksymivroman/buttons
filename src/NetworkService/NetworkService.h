//
// Created by rmaks on 26.02.2023.
//

#ifndef EVENT_BUTTON_NETWORKSERVICE_H
#define EVENT_BUTTON_NETWORKSERVICE_H

#include <Arduino.h>
#include "Global/Global.hpp"
#include <map>

typedef std::map<WiFiPhyMode, String> WiFiModeMap;

class NetworkService {

public:
    void setWiFiMode(BUTTON_WIFI_MODE mode);
    void ButtonHotspot(bool isOn, const char* ssid, const char* pass);
    void ConnectToWiFi(const String& ssid, const String& pass);
    NETWORKLIST WiFiList();
    bool isConnectedToWiFi();
    bool isAPMode();

    void initConnectionToWiFi(const WiFiCONFIG& config);

    String getWiFIMode() const;
    String ipAddress() const;

private:
    void initWirelessModule();
    BUTTON_WIFI_MODE _mode = AUTO;

    WiFiModeMap _modeMap = {
            {WIFI_PHY_MODE_11B, "11B"},
            {WIFI_PHY_MODE_11G, "11G"},
            {WIFI_PHY_MODE_11N, "11N"}
    };

    bool _isConnectionInitialized {false};
};


#endif //EVENT_BUTTON_NETWORKSERVICE_H
