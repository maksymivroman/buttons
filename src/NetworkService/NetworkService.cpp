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
    Serial.printf("[NetworkService] hostname: %s\n", WiFi.hostname().c_str());
    WiFi.begin(ssid, pass);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print("[NetworkService] Connecting to ");
        Serial.print(ssid); Serial.print(" / "); Serial.println(pass);
    }
    Serial.println(WiFi.localIP());
    Serial.printf("hostname: %s\n", WiFi.hostname().c_str());
}

void NetworkService::ButtonHotspot(bool isOn, const char* ssid, const char* pass) {
    if (isOn) {
        WiFi.mode(WIFI_STA);
        WiFi.softAP(ssid, pass);
        IPAddress IP = WiFi.softAPIP();
        Serial.print("[NetworkService] Set AP: ");Serial.print(ssid);Serial.print(". IP address: ");
        Serial.println(IP);
    } else {
        WiFi.softAPdisconnect(isOn);
        Serial.println("[NetworkService] AP is off.");
    }
}

NETWORKLIST NetworkService::WiFiList() {
    NETWORKLIST list;
    list.size = WiFi.scanNetworks();
    list.arr = new String[list.size];

    Serial.print("[NetworkService] Networks found: "); Serial.println(list.size);

    for (int i = 0 ; i < list.size; i++) {
        Serial.print("[NetworkService] SSID: "); Serial.println(WiFi.SSID(i));
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
