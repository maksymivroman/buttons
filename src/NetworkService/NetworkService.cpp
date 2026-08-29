//
// Created by rmaks on 26.02.2023.
//

#include "NetworkService.h"
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "Global/Global.hpp"

void NetworkService::setWiFiMode(BUTTON_WIFI_MODE mode) {
    this->_mode = mode;
}

void NetworkService::ConnectToWiFi(const String& ssid, const String& pass) {
    this->initWirelessModule();
    wifi_station_set_hostname("Event button");
    WiFi.setAutoConnect(false);
    logger.log("[NetworkService] Hostname: ", WiFi.hostname().c_str());
    logger.log("[NetworkService] Connecting to ", ssid, " ...");
    WiFi.begin(ssid, pass);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        logger.logSerial("[NetworkService] Connecting to ", ssid, " / ", pass);
    }
    logger.log("[NetworkService] IP: ", WiFi.localIP().toString(), " Hostname: ", WiFi.hostname().c_str());
}

void NetworkService::initConnectionToWiFi(const WiFiCONFIG &config) {
    auto ssid = config.ssid;
    auto pass = config.password;
    if (!this->_isConnectionInitialized) {
        this->initWirelessModule();
        wifi_station_set_hostname("Event button");
        WiFi.setAutoConnect(false);
        logger.log("[NetworkService] Hostname: ", WiFi.hostname().c_str());
        logger.log("[NetworkService] Connecting to ", ssid, " ...");
        WiFi.begin(ssid, pass);
        this->_isConnectionInitialized = true;
    }
}

void NetworkService::ButtonHotspot(bool isOn, const char* ssid, const char* pass) {
    if (isOn) {
        this->initWirelessModule();
        WiFi.softAP(ssid, pass);
        IPAddress IP = WiFi.softAPIP();
        logger.log("[NetworkService] Set AP: ", ssid, " IP address: ", IP.toString());
    } else {
        WiFi.softAPdisconnect(isOn);
        logger.log("[NetworkService] AP is off.");
    }
}

void NetworkService::getNetworksJson(JsonObject &result) {
    int16_t n = WiFi.scanComplete();

    if (n == WIFI_SCAN_FAILED) {
        WiFi.scanNetworks(true);
        n = 0;
    } else if (n == WIFI_SCAN_RUNNING) {
        n = 0;
    }

    logger.log("[NetworkService] Networks found: ", n);

    JsonArray networks = result.createNestedArray("networks");
    for (int16_t i = 0; i < n; ++i) {
        JsonObject item = networks.createNestedObject();
        item["ssid"] = WiFi.SSID(i);
        item["rssi"] = WiFi.RSSI(i);
        item["secure"] = (WiFi.encryptionType(i) != ENC_TYPE_NONE);
        logger.logSerial("[NetworkService] SSID: ", WiFi.SSID(i).c_str());
    }

    if (n > 0) {
        WiFi.scanDelete();
    }
}

bool NetworkService::isConnectedToWiFi() {
    return WiFi.isConnected();
}

bool NetworkService::isAPMode() {
    return (WiFi.getMode() == WIFI_AP) | (WiFi.getMode() == WIFI_AP_STA);
}

void NetworkService::initWirelessModule() {
    WiFi.mode(WIFI_STA);
    if (this->_mode) {
        logger.log("[NetworkService] Set WiFi mode to: ", this->_modeMap.at(static_cast<const WiFiPhyMode>(this->_mode)));
        WiFi.setPhyMode(static_cast<WiFiPhyMode_t>(this->_mode));
    }
}

String NetworkService::getWiFIMode() const {
    return this->_modeMap.at(WiFi.getPhyMode());
}

String NetworkService::ipAddress() const {
    switch (WiFi.getMode()) {
        case WIFI_STA:
            return WiFi.localIP().toString();
        case WIFI_AP:
        case WIFI_AP_STA:
            return WiFi.softAPIP().toString();
        default:
            return "unknown";
    }
}
