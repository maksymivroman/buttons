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
    ProcessToSend();
}

void EventsService::SendEventsOnKeystoreChange() {
    ProcessToSend(true);
}

void EventsService::ProcessToSend(boolean onlyFromKeystore) {
    logger.log("[EventsService] SendEvents");
    logger.logSerial("[EventsService] SendEvents", events);

    StaticJsonDocument<900> doc;
    deserializeJson(doc, events);
    JsonObject data = doc.as<JsonObject>();

    String requestUrl;

    for (JsonPair keyValue: data) {
        requestUrl = keyValue.key().c_str();

        String requestData = data[keyValue.key().c_str()];

        logger.logSerial("[EventsService] request data: ", requestData);
        logger.logSerial("[EventsService] request URL: ", requestUrl);

        UpdateEventDataWithKeystore(requestData, this->keystoreProp);

        if (requestUrl == "telegram" && !onlyFromKeystore) {
            SendMessageToTelegram(requestData);
        } else if (onlyFromKeystore && requestUrl.substring(0, 1) == "$") {
            requestUrl.remove(0, 1);
            SendHttpEvent(requestUrl, requestData);
        } else {
            if(requestUrl.substring(0, 1) != "$" && !onlyFromKeystore) {
                SendHttpEvent(requestUrl, requestData);
            }
        }
    }
}

void EventsService::SetEvents(String eventsData) {
    events = std::move(eventsData);
}

void EventsService::SendMessageToTelegram(String message) {
    logger.logSerial("[EventsService] SendMessageToTelegram: ", message);

    if (telegramBotRef != nullptr) {
        logger.log("[EventsService] Telegram: Integration Enabled. Sending message...");
        telegramBotRef->sendMessage(message);
    } else {
        logger.log("[EventsService] Telegram: Integration Disabled");
    }

}

void EventsService::SendHttpEvent(String &host, String &payload) {
    logger.log("[EventsService] HTTP request");
    logger.logSerial("[EventsService] HOST: ", host);
    logger.logSerial("[EventsService] PAYLOAD: ", payload);
    const bool isSecure = host.substring(4,5) == "s";

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
    } else {
        logger.log("[EventsService] Skip secure connection!");
    }
}

void EventsService::UpdateEventProp(String prop) {
    logger.log("[EventsService] Update event Prop: ", prop);
    this->keystoreProp = std::move(prop);
}

void EventsService::UpdateEventDataWithKeystore(String &eventData, String &replaceWith) {
    eventData.replace(this->propPattern, replaceWith);
}


