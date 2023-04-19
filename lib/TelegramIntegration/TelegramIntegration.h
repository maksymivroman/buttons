//
// Created by rmaks on 17.04.2023.
//

#ifndef INDEX_HTML_TELEGRAMINTEGRATION_H
#define INDEX_HTML_TELEGRAMINTEGRATION_H

#include <Arduino.h>
#include <BearSSLHelpers.h>
#include <WiFiClientSecure.h>
#include <ESP8266WiFi.h>
#include <utility>
#include <AsyncTelegram2.h>
#include "tg_certificate.h"

#include "GlobalConfig.hpp"

class TelegramIntegration {

public:
    void configureTelegramIntegration(INTEGRATIONSETTINGS config);

    void sendMessage(const String &message);

private:
    INTEGRATIONSETTINGS settings;

};


#endif //INDEX_HTML_TELEGRAMINTEGRATION_H
