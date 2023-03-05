//
// Created by rmaks on 15.02.2023.
//

#include <Arduino.h>
#include <ESPAsyncWebServer.h>
//#include <EEPROM.h>
#include "ArduinoJson.h"
#include "html-page.hpp"
#include "ButtonWebServer.h"
//#include "GlobalConfig.hpp"
#include "SettingsService.h"

//SettingsService ButtonSettings;
WiFiCONFIG wiFiConfig;

String postMessage;

void ButtonWebServer::startServer() {
    AsyncWebServer _server(80);

    Serial.println("[AsyncWebServer]");
    _server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        Serial.println("[AsyncWebServer] index_html");
        request->send_P(200, "text/html", index_html, wiFiCredits);
    });

    _server.on("/save", HTTP_GET, [this](AsyncWebServerRequest *request){
        request->send(200, "text/html", "<!DOCTYPE html><html lang=\"en\"><head> <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"> <title>Title</title> <style> html{ color: #333333; font-size: 18px} .container{ background: #565f60; display: flex; flex-direction: column; align-content: center; align-items: center;} @media screen and (max-width: 1080px) { .wifi-credentials{flex-direction: column} .main-container{width: 60vw} } </style></head><body class=\"container\"> <h1 style=\"color:#ced4da\">hooray, dude!</h1> <h2 style=\"color:#ddf2ff\">Button was successfully configured!</h2> <p></p> <textarea cols=120 rows=15 class=\"control\" style=\"margin-bottom: 1rem\">"
                                        + postMessage +  "</textarea></body></html>");
        Serial.println("restart ESP...");
        delay(2000);
        requireRestart = true;
    });

    // Route for POST /
//    _server.on("/", HTTP_POST, [this](AsyncWebServerRequest *request){
//        Serial.println("recive POST");
//        int params = request->params();
//        Serial.println(params);
//        if (params>=0){
//            AsyncWebParameter* p = request->getParam(0);
//            postMessage=p->value().c_str();
//            Serial.println(postMessage);
//        }
////        ButtonSettings.
//
////        saveConfig();
//        /////////////////////////////SAVE CONFIG
//        StaticJsonDocument<900> doc_eeprom;
//        deserializeJson(doc_eeprom, postMessage);
//        JsonObject data = doc_eeprom["inputdata"];
//        Serial.println(data);
//        const char* wifiname = data["wifiname"];
//        const char* wifipass = data["wifipass"];
//        const char* eventjson = data["eventdata"];
//
//        Serial.println("POST//json large: ");
//        Serial.println(data.size());
//
//        wiFiConfig.ssid = wifiname;
//        wiFiConfig.password = wifipass;
//        ButtonSettings.saveWiFiSettings(wiFiConfig);
//        ButtonSettings.saveEvents(eventjson);
//
//        Serial.println("send request...");
//        request->send(200, "text/html", "done");
//        //request->redirect("/save");
//    });

    _server.on("/getID", HTTP_POST, [](AsyncWebServerRequest *request){
        request->send(200, "text/html", WiFi.hostname().c_str());
    });

//    _server.onNotFound([](AsyncWebServerRequest *request) {
//        request->send(404, "text/plain", "Not found");
//});

    Serial.println("[AsyncWebServer] begin");
    _server.begin();
}

 String ButtonWebServer::wiFiCredits(const String& var) {
        //Serial.println(var);
    if(var == "BUTTONPLACEHOLDER"){
        String buttons = "";
//        buttons += "<div class=\"main-container\">";
//        buttons += "<label class=\"label\" for=\"wifiname\">WIFI name</label>";
//        buttons += "<input class=\"control\" type=\"text\" id=\"wifiname\" value=\""+inputSSID+"\"></div>";
//
//        buttons += "<div class=\"main-container\">";
//        buttons += "<label class=\"label\" for=\"wifipass\">WIFI password</label>";
//        buttons += "<input class=\"control\" type=\"password\" id=\"wifipass\" value=\""+inputPASS+"\"></div>";
//        buttons += "<label style=\"display:none;\" id=\"savedJSON\">" + jsonStr + "</label>";
//        buttons+=wifilist;

        return buttons;
    }
    return String();
}


//void ButtonWebServer::notFound(AsyncWebServerRequest *request) {
//    request->send(404, "text/plain", "Not found");
//}



