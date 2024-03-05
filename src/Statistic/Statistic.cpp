//
// Created by rmaks on 31-Jan-24.
//

#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include "Statistic.h"
#include "SettingsService/SettingsService.h"
#include "Global/Global.hpp"
#include "Global/Version.h"

extern Version currentFWVersion;
extern SettingsService buttonSettings;

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

        const String payload = this->prepareData(data);
        int result;
        if (isSecure) {
            logger.logSerial("[Statistic] Secure connection");
//            logger.logSerial("[Statistic] Skip secure connection. Try to use http...");
//            String url = this->apiUrl;
//            url.remove(4, 1);
            result = this->httpsPost(this->apiUrl, payload);
        } else {
            result = this->httpPost(this->apiUrl, payload);
        }

        if (result == -1) {
            return logger.log("[Statistic] Failed to send stat (HTTPC_ERROR_CONNECTION_FAILED)");
        }
        logger.log("[Statistic] Send stat result code: ", result);
    }
}

String Statistic::prepareData(const String &data) {

    String statData = R"({ "deviceID":"<deviceID>", "host":"<host>", "IP":"<ip>", "MAC":"<mac>", "data":"<data>" <extended> })";
    statData.replace("<deviceID>", buttonSettings.deviceID());
    statData.replace("<host>", WiFi.hostname().c_str());
    statData.replace("<ip>", buttonSettings.localIPAddress());
    statData.replace("<mac>", buttonSettings.macAddress());
    statData.replace("<data>", data);

    String extendedData = this->sendExtendedData ? this->extendedData() : "";
    statData.replace("<extended>", extendedData);

    return statData;
}

String Statistic::extendedData() {
    String extendedData = R"(, "firmwareVersion":"<FW>", "hotspotSSID":"<hotspotSSID>", "remoteButtonTriggering":<remoteButtonTriggering>, "clientWebAccessEnabled":<clientWebAccessEnabled>, "loggerEnabled":<loggerEnabled>)";
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
    std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
    client->setInsecure();
    HTTPClient https;

    if(https.begin(*client, url)) {
        https.addHeader("Content-Type", "application/json");
        https.setUserAgent("eButton");

        int httpsCode = https.POST(payload);
        https.end();
        return httpsCode;
    }
    return 0;
}
