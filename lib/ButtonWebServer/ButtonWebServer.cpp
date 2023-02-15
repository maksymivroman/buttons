//
// Created by rmaks on 15.02.2023.
//

#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <EEPROM.h>
#include "ArduinoJson.h"
#include "html-page.hpp"
#include "ButtonWebServer.h"
#include "GlobalConfig.hpp"

void ButtonWebServer::startServer(int port) {
    AsyncWebServer server(port);

    // Route for root / web page
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send_P(200, "text/html", index_html, wificredits);
    });

    server.on("/save", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(200, "text/html", "<!DOCTYPE html><html lang=\"en\"><head> <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"> <title>Title</title> <style> html{ color: #333333; font-size: 18px} .container{ background: #565f60; display: flex; flex-direction: column; align-content: center; align-items: center;} @media screen and (max-width: 1080px) { .wifi-credentials{flex-direction: column} .main-container{width: 60vw} } </style></head><body class=\"container\"> <h1 style=\"color:#ced4da\">hooray, dude!</h1> <h2 style=\"color:#ddf2ff\">Button was successfully configured!</h2> <p></p> <textarea cols=120 rows=15 class=\"control\" style=\"margin-bottom: 1rem\">"
                                        + postMessage +  "</textarea></body></html>");
        Serial.println("restart ESP...");
        delay(2000);
        restart=true;
    });

    // Route for POST /
    server.on("/", HTTP_POST, [](AsyncWebServerRequest *request){
        Serial.println("recive POST");
        int params = request->params();
        Serial.println(params);
        if (params>=0){
            AsyncWebParameter* p = request->getParam(0);
            postMessage=p->value().c_str();
            Serial.println(postMessage);
        }


//        saveConfig();
        /////////////////////////////SAVE CONFIG
        StaticJsonDocument<900> doc_eeprom;
        deserializeJson(doc_eeprom, postMessage);
        JsonObject data = doc_eeprom["inputdata"];
        Serial.println(data);
        const char* wifiname = data["wifiname"];
        const char* wifipass = data["wifipass"];
        const char* eventjson = data["eventdata"];

        Serial.println("POST//json large: ");
        Serial.println(data.size());
        /////////////////EEPROM write////////////////////////////////
        qsid = wifiname;
        qpass = wifipass;
        //qeventhost = eventhost;
        qevents = eventjson;

        if (qsid.length() > 0 && qpass.length() > 0) {
            Serial.println("POST//clearing eeprom");
            for (int i = 0; i < 1024; ++i) {
                EEPROM.write(i, 0);
            }
            Serial.println(qsid);
            Serial.println("");
            Serial.println(qpass);
            Serial.println("");

            Serial.println("POST//writing eeprom ssid:");
            for (int i = 0; i < qsid.length(); ++i)
            {
                EEPROM.write(i, qsid[i]);
                Serial.print("POST//Wrote: ");
                Serial.println(qsid[i]);
            }
            Serial.println("POST//writing eeprom pass:");
            for (int i = 0; i < qpass.length(); ++i)
            {
                EEPROM.write(32 + i, qpass[i]);
                Serial.print("POST//Wrote: ");
                Serial.println(qpass[i]);
            }

            EEPROM.commit();
        };
        ////////////////////WRITE SPIFS

        File file = SPIFFS.open("/post.json", "w");
        int bytesWritten = file.print(postMessage);
        file.close();

        // Print values.
        Serial.println(wifiname);
        Serial.println(wifipass);
        Serial.println("POST//bytesWritten:");
        Serial.println(bytesWritten);
        //////end JSON
        //////////////////////////////////////SAVE CONFIG





        Serial.println("send request...");
        request->send(200, "text/html", "done");
        //request->redirect("/save");
    });

    server.on("/getID", HTTP_POST, [](AsyncWebServerRequest *request){
        request->send(200, "text/html", WiFi.hostname().c_str());
    });

    // Send a GET request to <host_IP>/get?
    server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request) {
        String inputMessage;
        String inputParam;
        ///////////////GET DATA SAMPLE////////////////
        // {
        //   "inputdata": {
        //     "wifiname": "micro",
        //     "wifipass": "pass",
        //     "eventdata": {
        //       "event": "{\"eventName\":\"DYNAMIC_EVENT\"}",
        //       "event1": "{\"eventName\":\"DYNAMIC_EVENT\"}",
        //       "event2": "{\"eventName\":\"DYNAMIC_EVENT\"}"
        //     }
        //   }
        // }
        ////////////////////////////////////////

        // GET input value on <ESP_IP>/get?input
        if (request->hasParam(PARAM_INPUT)) {
            inputMessage = request->getParam(PARAM_INPUT)->value();
            inputParam = PARAM_INPUT;

            StaticJsonDocument<900> doc_eeprom;
            deserializeJson(doc_eeprom, inputMessage);
            JsonObject data = doc_eeprom["inputdata"];
            Serial.println(data);
            const char* wifiname = data["wifiname"];
            const char* wifipass = data["wifipass"];
            const char* eventjson = data["eventdata"];

            //const char* eventhost = data["eventhost"];
            //const char* events = data["events"];
            //Serial.println(events);

            Serial.println("json large: ");
            Serial.println(data.size());
            /////////////////EEPROM write////////////////////////////////
            qsid = wifiname;
            qpass = wifipass;
            //qeventhost = eventhost;
            qevents = eventjson;

            if (qsid.length() > 0 && qpass.length() > 0) {
                Serial.println("clearing eeprom");
                for (int i = 0; i < 1024; ++i) {
                    EEPROM.write(i, 0);
                }
                Serial.println(qsid);
                Serial.println("");
                Serial.println(qpass);
                Serial.println("");

                Serial.println("writing eeprom ssid:");
                for (int i = 0; i < qsid.length(); ++i)
                {
                    EEPROM.write(i, qsid[i]);
                    Serial.print("Wrote: ");
                    Serial.println(qsid[i]);
                }
                Serial.println("writing eeprom pass:");
                for (int i = 0; i < qpass.length(); ++i)
                {
                    EEPROM.write(32 + i, qpass[i]);
                    Serial.print("Wrote: ");
                    Serial.println(qpass[i]);
                }

                EEPROM.commit();
            };
            ////////////////////WRITE SPIFS

            File file = SPIFFS.open("/post.json", "w");
            int bytesWritten = file.print(inputMessage);
            file.close();

            // Print values.
            Serial.println(wifiname);
            Serial.println(wifipass);
            Serial.println("bytesWritten:");
            Serial.println(bytesWritten);
            //////end JSON
        }

        else {
            inputMessage = "GET// unvalid setting request";
            inputParam = "none";
        }

        Serial.println(inputMessage);
        // request->send(200, "text/html", "<h2>button will reset now with parameters:</h2><h6>"
        //                                 + inputMessage + "</h6>");
        request->send(200, "text/html", "<h2>button will reset now with parameters:</h2><h6>"
                                        + inputMessage + "</h6>");

        Serial.println("restart ESP...");
        delay(1000);
        restart=true;
    });

//    server.onNotFound(notFound);
    server.begin();
}

//void ButtonWebServer::notFound(AsyncWebServerRequest *request) {
//    request->send(404, "text/plain", "Not found");
//}
//
//void ButtonWebServer::saveConfig(){
//
//}



