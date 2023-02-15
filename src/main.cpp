#include <Arduino.h>
#include <ESP8266WiFi.h>

#include <ESP8266HTTPClient.h>
#include "ArduinoJson.h"
#include <EEPROM.h>
#include <DNSServer.h>
//New
#include "LEDService.h"
#include "html-page.hpp"
#include "ButtonWebServer.h"
#include "GlobalConfig.hpp"

//new
const int bluePin = 12;
const int greenPin = 13;
const int redPin = 14;
const int serverPort = 80;

LEDService ledService;


DNSServer dnsServer;


void scanwifinetwork(){
    wifilist="<div class=\"main-container\">";
    wifilist+="<label class=\"label\" for=\"wifiname\">WIFI List</label>";
    wifilist+="<select class=\"control\" id=\"networks\" onChange=\"update()\">";
    Serial.print("Scan start ... ");
    int n = WiFi.scanNetworks();
    Serial.print(n);
    Serial.println(" network(s) found");
    for (int i = 0; i < n; i++)
    {
        Serial.println(WiFi.SSID(i));
        wifilist+="<option value=\"" + WiFi.SSID(i)  +"\">" + WiFi.SSID(i) + "</option>";
    }
    wifilist+="</select></div>";
}



void setup() {
    ledService.pinConfig(redPin,greenPin,bluePin);




    //    uint8_t newMACAddress[] = {0x30, 0xAE, 0xA4, 0x07, 0x0D, 0x66};
    //  String newHostname = "button01";
    //
    //  wifi_set_macaddr(STATION_IF, &newMACAddress[0]);
    //   WiFi.hostname(newHostname.c_str());
    //  WiFi.mode(WIFI_STA);


    pinMode(output, OUTPUT);
    digitalWrite(output, LOW);

    pinMode(buttonPin, INPUT);
    pinMode(outputGreen, OUTPUT);
    //digitalWrite(outputGreen, LOW);

    digitalWrite(outputGreen, HIGH);
    delay(1000);
    digitalWrite(outputGreen, LOW);
    Serial.begin(115200);
    EEPROM.begin(1024);
    Serial.println("start SPIFFS");
    bool success = SPIFFS.begin();
    if (success) {
        Serial.println("SPIFFS// File system mounted with success");
    } else {
        Serial.println("SPIFFS// Error mounting the file system");
    }

    //File file = SPIFFS.open("/file.txt", "w");
    File file = SPIFFS.open("/post.json", "r");

    if (!file) {
        Serial.println("SPIFFS// Error opening file for writing");
        Serial.println("SPIFFS// creating new file...");
        File file = SPIFFS.open("/post.json", "w");
        int bytesWritten = file.print("");
        file.close();
        // if (bytesWritten > 0) {
        //   Serial.println("File was written");
        //   Serial.println(bytesWritten);
        //   }
        //   else {
        //     Serial.println("File write failed");
        //   }
    }
    else{

        //File file = SPIFFS.open("/post.json", "r");
        while (file.available()) {
            spiff_cont+=char(file.read());
        }
        Serial.println("SPIFFS// file ./post.json:");
        Serial.println(spiff_cont);
        file.close();
    }
    //deserialize
    Serial.println("deserializeJson// post.json");
    StaticJsonDocument<900> eep;
    deserializeJson(eep, spiff_cont);
    JsonObject data = eep["inputdata"];
    Serial.println("Json// {inputdata}:");
    const char* wifiname = data["wifiname"];
    const char* wifipass = data["wifipass"];
    const char* eventdata = data["eventdata"];
    Serial.println(data.size());

    StaticJsonDocument<900> spif;
    deserializeJson(spif, spiff_cont);
    JsonObject evdata = spif["inputdata"]["eventdata"];
    Serial.println("Json// {inputdata}:");
    // String jsonStr;
    serializeJson(evdata, jsonStr);
    Serial.println(jsonStr);

    Serial.println("SPIIFFS>Json// {inputdata} size: ");
    int evdatasize = evdata.size();

    Serial.println(evdatasize);
    Serial.println("SPIIFFS>Json// {inputdata} entris: ");

    ///////////
    for (JsonPair keyValue : evdata) {
        Serial.println(keyValue.key().c_str());
    }
    ////////////////////

    Serial.println("Reading EEPROM ssid");
    String esid;
    for (int i = 0; i < 32; ++i)
    {
        if(EEPROM.read(i)!=0){
            esid += char(EEPROM.read(i));
        }
    }
    inputSSID=esid;
    Serial.println();
    Serial.print("SSID: ");
    Serial.println(esid);
    Serial.println("Reading EEPROM pass");

    String epass = "";
    for (int i = 32; i < 96; ++i)
    {
        if(EEPROM.read(i)!=0){
            epass += char(EEPROM.read(i));
        }
    }
    inputPASS=epass;
    Serial.print("PASS: ");
    Serial.println(epass);

    // String ehost = "";
    //   for (int i = 96; i < 224; ++i)
    //   {
    //     ehost += char(EEPROM.read(i));
    //   }
    // Serial.print("HOST: ");
    // Serial.println(ehost);

    // String eevent = "";
    //   for (int i = 224; i < 512; ++i)
    //   {
    //     eevent += char(EEPROM.read(i));
    //   }
    // Serial.print("EVENT: ");
    // Serial.println(eevent);

    //   qeventhost = ehost;
    //   qevents = eevent;

    //==========if btn is presed then >CONFIG MODE and wifi hotspot is active untill reboot


    int reading = digitalRead(buttonPin);

    if (reading ==0){
        onConfig=true;

        ledService.blinkPrimary();
        ledService.lightOnGreen(true);

        Serial.println(">>>CONFIG MODE<<<");
        scanwifinetwork();
        Serial.println("Setting soft-AP ... ");
        WiFi.softAP("BUTTON_CONFIG", "12345678");
        IPAddress IP = WiFi.softAPIP();
        Serial.print("AP IP address: ");
        Serial.println(IP);
        //SET DNS
        dnsServer.setTTL(300);
        dnsServer.setErrorReplyCode(DNSReplyCode::ServerFailure);
        dnsServer.start(DNS_PORT, "www.btn.net", IP);

    }
    else{
        // Connect to Wi-Fi
        //WiFi.hostname(hostname.c_str()); //define hostname

        WiFi.mode(WIFI_STA);
        wifi_station_set_hostname(host);
        WiFi.setAutoConnect(false);
        Serial.printf("hostname: %s\n", WiFi.hostname().c_str());
        WiFi.begin(esid, epass);
        while (WiFi.status() != WL_CONNECTED) {
            delay(1000);
            Serial.println("Connecting to WiFi..");
            digitalWrite(output,!digitalRead(output));
        }
        digitalWrite(output, LOW);
        digitalWrite(outputGreen, HIGH);
    }
    // Print ESP Local IP Address
    Serial.println(WiFi.localIP());
    Serial.printf("hostname: %s\n", WiFi.hostname().c_str());
    //HTML





}

void loop() {
// write your code here

    if (restart){
        unsigned long timing= millis ();
        Serial.println("restart in 3 sec...");
        delay(3000);
        Serial.println("restart...");
        ESP.restart();
        //setup();
    }
    if (onConfig!=false){
        dnsServer.processNextRequest();
    }
    int reading = digitalRead(buttonPin);
    if (onConfig==false){

        if (reading ==0) {
            Serial.println("btn");
            digitalWrite(outputGreen, LOW);
            ledService.blinkPrimary();

            StaticJsonDocument<900> main;
            deserializeJson(main, spiff_cont);
            JsonObject maindata = main["inputdata"]["eventdata"];
            String keyval;

            for (JsonPair keyValue : maindata) {
                keyval=keyValue.key().c_str();
                Serial.println(keyValue.key().c_str());

                String postname = maindata[keyValue.key().c_str()];
                //serializeJson(postname, jsonStr);
                Serial.println(postname);

                Serial.println(">HTTP DATA: " + keyval + "/" + postname );
                char toHost[128];
                keyval.toCharArray(toHost, 128);
                char toEvent[128];
                postname.toCharArray(toEvent, 128);

                String ifsecure = keyval.substring(4,5);
                if (ifsecure=="s"){
                    Serial.println("secure");
                    std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
                    client->setInsecure();
                    HTTPClient http;
                    http.begin(*client, toHost);           //////qeventhost or "https://webhook.site/71c2f7ef-1dff-492b-9377-5757859cb3c4"
                    http.addHeader("Content-Type", "application/json");
                    http.setUserAgent( "button_prototype");
                    int httpCode = http.POST(toEvent);    ///////// "{\"eventName\":\"DYNAMIC_EVENT\"}"
                    Serial.println(httpCode);
                    http.end();
                }
                else{
                    Serial.println("not secure");
                    WiFiClient client;
                    HTTPClient http;
                    http.begin(client, toHost);         //http://192.168.2.71:55554/event
                    http.addHeader("Content-Type", "application/json");
                    http.setUserAgent( "button_prototype");
                    int httpCode = http.POST(postname);
                    Serial.println(httpCode);
                    http.end();
                }
            }

            ledService.blinkWarn();
            ledService.blinkDone();
        }
        if (WiFi.status() != WL_CONNECTED){
            Serial.println(">>>disconected...");
            Serial.println("restart in 3 sec...");
            delay(3000);
            Serial.println("restart...");
            ESP.restart();
        }
    }
}