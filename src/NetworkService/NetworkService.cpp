//
// Created by rmaks on 26.02.2023.
//

#include "NetworkService.h"
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "Global/Global.hpp"

void NetworkService::ConnectToWiFi(const String& ssid, const String& pass) {
    WiFi.mode(WIFI_STA);
    wifi_station_set_hostname("Event button");
    WiFi.setAutoConnect(false);
    logger.log("[NetworkService] Hostname: ", WiFi.hostname().c_str());
    logger.log("[NetworkService] Connecting to", ssid, " ...");
    WiFi.begin(ssid, pass);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        logger.logSerial("[NetworkService] Connecting to ", ssid, " / ", pass);
    }
    logger.log("[NetworkService] IP: ", WiFi.localIP().toString(), " Hostname: ", WiFi.hostname().c_str());
}

void NetworkService::ButtonHotspot(bool isOn, const char* ssid, const char* pass) {
    if (isOn) {
        WiFi.mode(WIFI_STA);
        WiFi.softAP(ssid, pass);
        IPAddress IP = WiFi.softAPIP();
        logger.log("[NetworkService] Set AP: ", ssid, " IP address: ", IP.toString());
    } else {
        WiFi.softAPdisconnect(isOn);
        logger.log("[NetworkService] AP is off.");
    }
}

NETWORKLIST NetworkService::WiFiList() {
    NETWORKLIST list;
    list.size = WiFi.scanNetworks();
    list.arr = new String[list.size];

    logger.log("[NetworkService] Networks found: ", list.size);

    for (int i = 0 ; i < list.size; i++) {
        logger.logSerial("[NetworkService] SSID: ", WiFi.SSID(i).c_str());
        list.arr[i] = WiFi.SSID(i);
    }

    return list;
}

bool NetworkService::isConnectedToWiFi() {
    return WiFi.isConnected();
}

bool NetworkService::isAPMode() {
    return (WiFi.getMode() == WIFI_AP) | (WiFi.getMode() == WIFI_AP_STA);
}
