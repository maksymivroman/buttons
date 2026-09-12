//
// Created by rmaks on 07.03.2023.
//

#include <utility>
#include "ArduinoJson.h"
#include "EventsService.h"
#include <Arduino.h>
#include <ESP8266HTTPClient.h>
#include "Global/Global.hpp"

void EventsService::SetEvents(String eventsData, bool serialEnabled) {
    events = std::move(eventsData);
    this->_serialEnabled = serialEnabled;
}

/** new version of events -> actions */
static bool isTriggerMatch(ACTION_TRIGGER actionTrigger, const char *triggerStr) {
    if (!triggerStr) return false;
    switch (actionTrigger) {
        case ACTION_TRIGGER::SINGLE_PRESS:
            return strcmp(triggerStr, "SINGLE_PRESS") == 0;
        case ACTION_TRIGGER::SWITCH_ON:
            return strcmp(triggerStr, "SWITCH_ON") == 0;
        case ACTION_TRIGGER::SWITCH_OFF:
            return strcmp(triggerStr, "SWITCH_OFF") == 0;
        case ACTION_TRIGGER::KEYSTORE_UPDATE:
            return strcmp(triggerStr, "KEYSTORE_UPDATE") == 0;
        case ACTION_TRIGGER::REMOTE_TRIGGER:
            return strcmp(triggerStr, "REMOTE_TRIGGER") == 0;
        default:
            return false;
    }
}

void EventsService::performAction(ACTION_TRIGGER actionTrigger) {
    logger.log("[EventsService][performAction] Trigger: ", static_cast<int>(actionTrigger));

    if (this->events.isEmpty() || this->events == "{}") {
        logger.log("[EventsService][performAction] Events data is empty");
        return;
    }

    DynamicJsonDocument doc(4096);
    DeserializationError error = deserializeJson(doc, this->events);
    if (error) {
        logger.log("[EventsService][performAction] JSON deserialization failed: ", error.c_str());
        return;
    }

    if (!doc.is<JsonArray>()) {
        logger.log("[EventsService][performAction] Events data is not a JSON Array");
        return;
    }

    JsonArray eventsArray = doc.as<JsonArray>();
    for (JsonObject event : eventsArray) {
        bool enabled = event["enabled"] | true;
        if (!enabled) {
            continue;
        }

        JsonArray triggers = event["triggers"].as<JsonArray>();
        bool matched = false;
        for (const char *trigger : triggers) {
            if (isTriggerMatch(actionTrigger, trigger)) {
                matched = true;
                break;
            }
        }

        if (!matched) {
            continue;
        }

        this->executeActions(event);
    }
}

void EventsService::executeActions(const JsonObject &event) {
    String host = event["host"] | "";
    String payload = event["payload"] | "";
    JsonArray actions = event["actions"].as<JsonArray>();

    logger.log("[EventsService][performAction] Matched event: ", event["name"] | "");

    for (const char *action : actions) {
        if (!action) continue;
        if (strcmp(action, "HTTP_REQUEST") == 0) {
            this->httpRequest(host, payload);
        } else if (strcmp(action, "SERIAL_DATA") == 0) {
            this->serial(payload);
        } else if (strcmp(action, "OI_OUTPUT") == 0) {
            this->io();
        }
    }
}

ActionResult EventsService::httpRequest(const String &host, const String &payload) {
    if (!networkService.isConnectedToWiFi()) {
        logger.log("[EventsService][httpRequest] No WiFi connection. Skipping HTTP request");
        return FAILED;
    }

    const bool isValidUrl = host.startsWith("http://") || host.startsWith("https://");
    if (!isValidUrl) {
        logger.log("[EventsService][httpRequest] Invalid URL format: ", host);
        return FAILED;
    }

    logger.log("[EventsService][httpRequest] Sending HTTP request to: ", host);
    logger.logSerial("[EventsService][httpRequest] Payload: ", payload);

    WiFiClient client;
    HTTPClient http;

    if (!http.begin(client, host)) {
        logger.log("[EventsService][httpRequest] Failed to connect to host: ", host);
        return FAILED;
    }

    http.addHeader("Content-Type", "application/json");
    http.setUserAgent("eButton");

    int httpCode = http.POST(payload);
    logger.log("[EventsService][httpRequest] Response status code: ", httpCode);
    http.end();

    return (httpCode > 0 && httpCode < 400) ? SUCCESS : FAILED;
}

ActionResult EventsService::serial(const String &payload) {
    if (!this->_serialEnabled) {
        logger.log("[EventsService][serial] Events via Serial is disabled");
        return IGNORED;
    }

    logger.log("[EventsService][serial] Sending payload: ", payload);
    Serial.println(payload);
    return SUCCESS;
}
