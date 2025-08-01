//
// Created by rmaks on 07.03.2023.
//

#include <utility>
#include "ArduinoJson.h"
#include "EventsService.h"
#include <Arduino.h>
#include <ESP8266HTTPClient.h>
#include "Global/Global.hpp"

void EventsService::SendEvents(EVENT_TRIGGER triggeredBy) {
    logger.log("[EventsService] SendEvents with trigger: ", triggeredBy);
    logger.logSerial("[EventsService] SendEvents", this->events);
    ProcessEvents(triggeredBy);
}

void EventsService::SendEventsOnKeystoreChange() {
    ProcessEvents(KEYSTORE_UPDATE);
}

void EventsService::ProcessEvents(EVENT_TRIGGER triggeredBy) {
    StaticJsonDocument<900> doc;
    deserializeJson(doc, this->events);
    auto data = doc.as<JsonObject>();

    for (JsonPair keyValue: data) {
        String requestHost = keyValue.key().c_str();
        String requestData = data[keyValue.key().c_str()];

        logger.logSerial("[EventsService] request data: ", requestData);
        logger.logSerial("[EventsService] request URL: ", requestHost);

        if (triggeredBy == DEFAULT_TRIGGER) {
            auto isDefaultEvent = this->isDefaultEvent(requestHost);
            if (isDefaultEvent) this->ProcessToSend(requestHost, requestData);
        } else if (this->isEventMatchTrigger(triggeredBy, requestHost)) {
            auto host = this->normalizeRequestHost(requestHost, triggeredBy);
            this->ProcessToSend(host, requestData);
        }
    }
}

bool EventsService::isEventMatchTrigger(EVENT_TRIGGER trigger, const String &eventRequestHost) const {
    const auto t = _triggers.at(trigger);
    return std::equal(
            t.begin(), t.end(),
            eventRequestHost.c_str());
}

bool EventsService::isDefaultEvent(const String &eventRequestHost) {
    for (auto trigger:_triggers) {
        bool isEventWithTrigger = this->isEventMatchTrigger(trigger.first, eventRequestHost);
        if (isEventWithTrigger) return false;
    }
    return true;
}

String EventsService::normalizeRequestHost(String host, EVENT_TRIGGER trigger) {
    auto triggerStr = this->_triggers.at(trigger);
    host.remove(0, triggerStr.size());
    return host;
}

void EventsService::ProcessToSend(String &host, String &payload) {
    if (host == "serial") {
        if (this->_serialEnabled) {
            SendSerialEvent(payload);
        } else {
            logger.log("[EventsService] Events via Serial is disabled");
        }
    } else {
        if (networkService.isConnectedToWiFi()) {
            SendHttpEvent(host, payload);
        } else {
            logger.log("[EventsService] No WiFi connection. Skip to send HttpEvent");
        }
    }
}

void EventsService::SetEvents(String eventsData, bool serialEnabled) {
    events = std::move(eventsData);
    this->_serialEnabled = serialEnabled;
}

void EventsService::SendHttpEvent(String &host, String &payload) {
    logger.log("[EventsService][SendHttpEvent] HTTP request");
    logger.logSerial("[EventsService][SendHttpEvent] HOST: ", host);
    logger.logSerial("[EventsService][SendHttpEvent] PAYLOAD: ", payload);
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

void EventsService::SendSerialEvent(String &payload) {
    logger.log("[EventsService][SendSerialEvent]", payload);
    String data = payload + "\r\n";
    Serial.write(data.c_str());
}
