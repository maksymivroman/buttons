//
// Created by rmaks on 17.04.2023.
//

#include "TelegramIntegration.h"


#define MYTZ "CET-1CEST,M3.5.0,M10.5.0/3"

void TelegramIntegration::sendMessage(const String& message) {
    logger.log("[TelegramIntegration] -> Start sendMessage: ");
    logger.logSerial("[TelegramIntegration] -> Start sendMessage: ", message);

    BearSSL::WiFiClientSecure client;
    BearSSL::Session session;
    BearSSL::X509List certificate(telegram_cert);

    AsyncTelegram2 tBot(client, 1024);

    configTime(MYTZ, "time.google.com", "time.windows.com", "pool.ntp.org");

    client.setSession(&session);
    client.setTrustAnchors(&certificate);
    client.setBufferSizes(1024, 1024);

    tBot.setTelegramToken(settings.tToken.c_str());

    tBot.begin() ? logger.log("[TelegramIntegration] -> Init OK ") : logger.log("[TelegramIntegration] -> Failed to start");

    tBot.sendTo(settings.tChanelID , message);
}

void TelegramIntegration::configureTelegramIntegration(INTEGRATIONSETTINGS config) {
    settings = std::move(config);
}

