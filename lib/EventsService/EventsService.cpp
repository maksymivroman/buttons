//
// Created by rmaks on 07.03.2023.
//

#include <utility>
#include "ArduinoJson.h"
#include "EventsService.h"
#include <Arduino.h>
#include <ESP8266HTTPClient.h>


void EventsService::SendEvents() {
    Serial.print("[EventsService]"); Serial.println(events);

    StaticJsonDocument<900> doc;
    deserializeJson(doc, events);
    JsonObject data = doc.as<JsonObject>();

    String requestUrl;

    for (JsonPair keyValue: data) {
        requestUrl = keyValue.key().c_str();

        String requestData = data[keyValue.key().c_str()];

        Serial.print("[EventsService] request data: ");  Serial.println(requestData);
        Serial.print("[EventsService] request URL: ");  Serial.println(requestUrl);

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
    Serial.print("[EventsService] SendMessageToTelegram: "); Serial.println(message);

    if (telegramBotRef != nullptr) {
        Serial.println("[EventsService] SendMessageToTelegram: Integration Enabled. Sending message...");
        telegramBotRef->sendMessage(message);
    } else {
        Serial.println("[EventsService] SendMessageToTelegram: Integration Disabled");
    }

}

void EventsService::SendHttpEvent(String &host, String &payload) {
    Serial.println("[EventsService] HTTP request");

    Serial.print("[EventsService] HOST: "); Serial.println(host);
    Serial.print("[EventsService] PAYLOAD: "); Serial.println(payload);
    const bool isSecure = host.substring(4,5) == "s";

    Serial.print("[EventsService] HTTPS: "); Serial.println(isSecure);

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
        Serial.print("[EventsService] Response status code: "); Serial.println(httpCode);

        http.end();
    }
}
