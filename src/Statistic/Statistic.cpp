//
// Created by rmaks on 31-Jan-24.
//

#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include "Statistic.h"
#include "SettingsService/SettingsService.h"
#include "NetworkService/NetworkService.h"
#include "Global/Global.hpp"
#include "Global/Version.h"

extern Version currentFWVersion;
extern SettingsService buttonSettings;
extern NetworkService networkService;

void Statistic::initStat(String apiUrl, http_stat_method method, unsigned int statisticLevel) {
    this->apiUrl = apiUrl;
    this->sendExtendedData = (statisticLevel != 0);
    this->method = method;
    this->enabled = true;
}

void Statistic::sendStat(String data) {
    if (this->enabled) {
        logger.logSerial("[Statistic] Send stat: ", data);
        if (this->apiUrl.length() < 10) {
            logger.log("[Statistic] Wrong url: ", data);
            return;
        }

        const bool isSecure = this->apiUrl.substring(0, 5) == "https";

        const bool asUrlParams = this->method == STAT_HTTP_GET;
        const String payload = this->prepareData(data, asUrlParams);
        int result;
        if (isSecure) {
            logger.logSerial("[Statistic] Secure connection");
            result = this->method == STAT_HTTP_GET ? this->httpsGet(this->apiUrl, payload) : this->httpsPost(this->apiUrl, payload);
        } else {
            logger.logSerial("[Statistic] Unsecure connection");
            result = this->method == STAT_HTTP_GET ? this->httpGet(this->apiUrl, payload) : this->httpPost(this->apiUrl, payload);
        }

        if (result == -1) {
            return logger.log("[Statistic] Failed to send stat (HTTPC_ERROR_CONNECTION_FAILED)");
        }
        logger.log("[Statistic] Send stat result code: ", result);
    }
}

String Statistic::prepareData(const String &data, bool asUrlParams) {
    String statData;
    if (asUrlParams) {
        statData = R"(?deviceID=<deviceID>&host=<host>&IP=<ip>&MAC=<mac>&data=<data><extended>)";
    } else {
        statData = R"({ "deviceID":"<deviceID>", "host":"<host>", "IP":"<ip>", "MAC":"<mac>", "data":"<data>" <extended> })";
    }

    statData.replace("<deviceID>", buttonSettings.deviceID());
    statData.replace("<host>", WiFi.hostname().c_str());
    statData.replace("<ip>", networkService.ipAddress());
    statData.replace("<mac>", buttonSettings.macAddress());
    statData.replace("<data>", data);

    String extendedData = this->sendExtendedData ? this->extendedData(asUrlParams) : "";
    statData.replace("<extended>", extendedData);

    return statData;
}

String Statistic::extendedData(bool asUrlParams) {
    String extendedData;
    if (asUrlParams) {
        extendedData = R"(&firmwareVersion=<FW>&hotspotSSID=<hotspotSSID>&remoteButtonTriggering=<remoteButtonTriggering>&clientWebAccessEnabled=<clientWebAccessEnabled>&loggerEnabled=<loggerEnabled>)";
    } else {
        extendedData = R"(, "firmwareVersion":"<FW>", "hotspotSSID":"<hotspotSSID>", "remoteButtonTriggering":<remoteButtonTriggering>, "clientWebAccessEnabled":<clientWebAccessEnabled>, "loggerEnabled":<loggerEnabled>)";
    }
    extendedData.replace("<FW>", currentFWVersion.str_fullVersion());
    extendedData.replace("<hotspotSSID>", String(buttonSettings.customHotspotSsid()));
    extendedData.replace("<remoteButtonTriggering>", String(buttonSettings.remoteButtonTriggering()));
    extendedData.replace("<clientWebAccessEnabled>", String(buttonSettings.clientWebAccessEnabled()));
    extendedData.replace("<loggerEnabled>", String(buttonSettings.loggerEnabled()));

    return extendedData;
}

int Statistic::httpPost(String &url, const String &payload) {
    WiFiClient client;
    HTTPClient http;

    http.begin(client, url);
    http.addHeader("Content-Type", "application/json");
    http.setUserAgent("eButton");
    int httpCode = http.POST(payload);
    http.end();
    return httpCode;
}

int Statistic::httpsPost(String &url, const String &payload) {
    std::unique_ptr<BearSSL::WiFiClientSecure> client(new BearSSL::WiFiClientSecure);
    client->setInsecure();
    HTTPClient https;

    if (https.begin(*client, url)) {
        https.addHeader("Content-Type", "application/json");
        https.setUserAgent("eButton");

        int httpsCode = https.POST(payload);
        https.end();
        return httpsCode;
    }
    return 0;
}

int Statistic::httpGet(String &url, const String &payload) {
    WiFiClient client;
    HTTPClient http;
    String path = url + payload;
    http.begin(client, path.c_str());
    int httpResponseCode = http.GET();
    http.end();
    return httpResponseCode;
}

int Statistic::httpsGet(String &url, const String &payload) {
    std::unique_ptr<BearSSL::WiFiClientSecure> client(new BearSSL::WiFiClientSecure);
    client->setInsecure();
    HTTPClient https;
    String path = url + payload;
    https.begin(*client, path.c_str());
    int httpsResponseCode = https.GET();
    https.end();
    return httpsResponseCode;
}
