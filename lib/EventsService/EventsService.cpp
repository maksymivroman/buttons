//
// Created by rmaks on 07.03.2023.
//

#include <utility>
#include "ArduinoJson.h"
#include "EventsService.h"
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>


void EventsService::SendEvents() {

    Serial.print("[EventsService]"); Serial.println(events);

    StaticJsonDocument<900> doc;
    deserializeJson(doc, events);
    JsonObject data = doc.as<JsonObject>();

    String requestUrl;

    for (JsonPair keyValue : data) {
        requestUrl = keyValue.key().c_str();

        String requestData = data[keyValue.key().c_str()];

        Serial.print("[EventsService] request data: ");  Serial.println(requestData);
        Serial.print("[EventsService] request URL: ");  Serial.println(requestUrl);

        char host[128];
        requestUrl.toCharArray(host, 128);
        char event[128];
        requestData.toCharArray(event, 128);

        //TODO https not working !!!
        String ifSecure = requestUrl.substring(4, 5);
        if (ifSecure=="s"){
            Serial.println("secure");
            std::unique_ptr<BearSSL::WiFiClientSecure>sslClient(new BearSSL::WiFiClientSecure);
            Serial.println("BearSSL");
            sslClient->setInsecure();
            HTTPClient httpClient;
            httpClient.begin(*sslClient, host);
            httpClient.addHeader("Content-Type", "application/json");
            httpClient.setUserAgent( "button_prototype");
            Serial.println("*HTTPClient->POST");
            int httpCode = httpClient.POST(event);    // "{\"eventName\":\"DYNAMIC_EVENT\"}"
            Serial.println("*HTTPClient->POST result:");
            Serial.println(httpCode);
            Serial.println("*HTTPClient->end:");
            httpClient.end();
        }
        else{
            Serial.println("[EventsService] HTTP request");
            WiFiClient client;
            HTTPClient http;
            http.begin(client, host);
            http.addHeader("Content-Type", "application/json");
            http.setUserAgent( "button_prototype");
            int httpCode = http.POST(requestData);
            Serial.print("[EventsService] Response status code: "); Serial.println(httpCode);
            http.end();
        }
    }
}

void EventsService::SetEvents(String eventsData) {
    events = std::move(eventsData);
}
