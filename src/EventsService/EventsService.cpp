//
// Created by rmaks on 07.03.2023.
//

#include <utility>
#include "ArduinoJson.h"
#include "EventsService.h"
#include <Arduino.h>
#include <ESP8266HTTPClient.h>
#include "Global/Global.hpp"

void EventsService::SendEvents() {
    logger.log("[EventsService] ", events);

    StaticJsonDocument<900> doc;
    deserializeJson(doc, events);
    JsonObject data = doc.as<JsonObject>();

    String requestUrl;

    for (JsonPair keyValue: data) {
        requestUrl = keyValue.key().c_str();

        String requestData = data[keyValue.key().c_str()];

        logger.log("[EventsService] request data: ", requestData);
        logger.log("[EventsService] request URL: ", requestUrl);

        if (requestUrl == "telegram") {
            SendMessageToTelegram(requestData);
        } else {
            SendHttpEvent(requestUrl, requestData);
        }
    }
}

void EventsService::SetEvents(String eventsData) {
    events = std::move(eventsData);
}

void EventsService::SendMessageToTelegram(String message) {
    logger.log("[EventsService] SendMessageToTelegram: ", message);

    if (telegramBotRef != nullptr) {
        logger.log("[EventsService] SendMessageToTelegram: Integration Enabled. Sending message...");
        telegramBotRef->sendMessage(message);
    } else {
        logger.log("[EventsService] SendMessageToTelegram: Integration Disabled");
    }

}

void EventsService::SendHttpEvent(String &host, String &payload) {
    logger.log("[EventsService] HTTP request");
    logger.log("[EventsService] HOST: ", host);
    logger.log("[EventsService] PAYLOAD: ", payload);
    const bool isSecure = host.substring(4,5) == "s";

    logger.log("[EventsService] HTTPS: ", isSecure);

    //Skip secure connection
    if (!isSecure) {
        char hostUrl[128];
        host.toCharArray(hostUrl, 128);
        char event[128];
        payload.toCharArray(event, 128);

        WiFiClient client;
        HTTPClient http;

        http.begin(client, host);
        http.addHeader("Content-Type", "application/json");
        http.setUserAgent("eButton");
        int httpCode = http.POST(payload);
        logger.log("[EventsService] Response status code: ", httpCode);

        http.end();
    }
}
