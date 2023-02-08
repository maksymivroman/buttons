#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncWebServer.h>
#include <ESP8266HTTPClient.h>
#include "ArduinoJson.h"
#include <EEPROM.h>
#include <DNSServer.h>

const int output = 4;
const int outputGreen = 13;  //5
const int buttonPin = 5;  //2
const int ipPort = 80;
//DNS SETUP
const byte DNS_PORT = 53;
DNSServer dnsServer;


// Variables will change:
int ledState = LOW;
int buttonState;
int lastButtonState = LOW;

bool restart;
bool onConfig=false;
bool blinkID=false;

String qsid="";
String qpass="";
String qeventhost="";
String qevents="";
String spiff_cont ="";
String inputSSID ="";
String inputPASS ="";
String wifilist="";
String jsonStr;

String postMessage;
char host[] = "BUTTON protorype 2";

AsyncWebServer server(ipPort);
const char* PARAM_INPUT = "inputdata";

// HTML
const char index_html[] PROGMEM = R"rawliteral(
  <!DOCTYPE html>
  <html lang="en">
  <head>
      <meta name="viewport" content="width=device-width, initial-scale=1">
      <meta charset="UTF-8">
      <title>button config page</title>
      <style>
          html{ color: #333333; font-size: 16px}
          .container{ background: #565f60; display: flex; flex-direction: column; align-items: center;}
          .wifi-credentials{ display: flex; flex-direction: row; justify-content: center; }
          .main-container{display: flex; flex-direction: column; width: 20vw; margin: 1rem}
          .control {
              display: block;
              width: 100%;
              min-width: 220px;
              padding: .375rem .75rem;
              font-size: 1rem;
              line-height: 1.5;
              color: #495057;
              background-color: #fff;
              background-clip: padding-box;
              border: 1px solid #ced4da;
              border-radius: .25rem;
              transition: border-color .15s ease-in-out,box-shadow .15s ease-in-out;
          }
          .btn {
              color: #fff;
              background-color: #cb1d38;
              display: inline-block;
              font-weight: 400;
              text-align: center;
              white-space: nowrap;
              vertical-align: middle;
              -webkit-user-select: none;
              -moz-user-select: none;
              -ms-user-select: none;
              user-select: none;
              border: 1px solid #cb1d38;
              padding: .375rem .75rem;
              font-size: 1rem;
              line-height: 1.5;
              border-radius: .25rem;
              transition: color .15s ease-in-out,background-color .15s ease-in-out,border-color .15s ease-in-out,box-shadow .15s ease-in-out;
          }
          .btn:hover{
            background-color: #a6132f;
            transition: all 200ms;
          }
          .label {
              color: white;
              font-size: 1rem;
              display: inline-block;
              margin-bottom: .5rem;
              align-self: flex-start;
          }
          option {
              font-size: 1rem;
          }
          button, input, optgroup, select, textarea {
              margin: 0;
              font-family: inherit;
              font-size: inherit;
              line-height: inherit;
          }
          *, ::after, ::before {
              box-sizing: border-box;
          }
          button, select {
              text-transform: none;
          }
          select.control:not([size]):not([multiple]) {
              height: calc(2.25rem + 2px);
          }
          @media screen and (max-width: 1080px) {
              .wifi-credentials{flex-direction: column}
              .main-container{width: 60vw}
          }
      </style>

  </head>
  <body class="container">
      <h1 style="color:#ddf2ff">Button Setup Page</h1>

      <div class="wifi-credentials">
        %BUTTONPLACEHOLDER%
      </div>

      <div class="main-container" style="width: 60vw;">
          <textarea id="saved" cols=120 rows=15  class="control" style="margin-bottom: 1rem"></textarea>
          <button type="button" class="btn" onclick="sentcontent()">GET Save And Reboot</button>
          <p></p>
          <button type="button" class="btn" onclick="sentPOSTcontent()">POST Save And Reboot</button>
      </div>
  </body>
  </html>

  <script>
      function mergecontent(){
          //  var wifiname = document.getElementById('wifiname').value;
          //  var wifipass = document.getElementById('wifipass').value;
          //  var h1 = document.getElementById('host1').value;
          //  var h2 = document.getElementById('host2').value;
          //  var h3 = document.getElementById('host3').value;
          //  var e1 = document.getElementById('event1').value;
          //  var e2 = document.getElementById('event2').value;
          //  var e3 = document.getElementById('event3').value;

          //  let data='inputdata={ "inputdata" :{"wifiname":"' + wifiname + '","wifipass":"'+ wifipass +'","eventdata": {"'+
          //      h1 +'":"' + e1 +'",'+
          //      '"' + h2 +'":"'  + e2 +'",'+
          //      '"' + h3 +'":"' + e3 +'"}}}';

          // window.location.href = '/get?'+data;
          //console.log(data);
      }
      function sentcontent(){
          var name = document.getElementById('wifiname').value;
          var pass = document.getElementById('wifipass').value;

          data='/get?inputdata={ "inputdata" :{"wifiname":"' + name + '","wifipass":"'+ pass +'","eventdata":'+ document.getElementById('saved').value + '}';
          data.replace(/" /g, '');
          data.replace(/ "/g, '');
          window.location.href=data;
      }

          function sentPOSTcontent(){
          var name = document.getElementById('wifiname').value;
          var pass = document.getElementById('wifipass').value;

          data='{ "inputdata" :{"wifiname":"' + name + '","wifipass":"'+ pass +'","eventdata":'+ document.getElementById('saved').value + '}';
          data.replace(/" /g, '');
          data.replace(/ "/g, '');
          srvURL=window.location.protocol + "//" + window.location.host + "/";
            var pst = new XMLHttpRequest();
            pst.open("POST", srvURL, true);
            pst.setRequestHeader("Content-Type", "application/x-www-form-urlencoded; charset=UTF-8");
            pst.send(data);
            pst.responseType = 'text';
              pst.onreadystatechange = function() {
                if (pst.readyState === pst.DONE) {
          if (pst.status === 200) {
              console.log(pst.response);
              window.location.href="/save";
              //console.log(pst.responseText);
          }
      }
            }
          //window.location.href="/save";
      }

      function showJSON() {
            var wifiname = document.getElementById('wifiname').value;
          var wifipass = document.getElementById('wifipass').value;
          var h1 = document.getElementById('host1').value;
          var h2 = document.getElementById('host2').value;
          var h3 = document.getElementById('host3').value;
          var e1 = document.getElementById('event1').value;
          var e2 = document.getElementById('event2').value;
          var e3 = document.getElementById('event3').value;
              var ugly ='{ "inputdata" :{"wifiname":"' + wifiname + '","wifipass":"'+ wifipass +'","eventdata": {"'+
              h1 +'":"' + e1 +'",'+
              '"' + h2 +'":"'  + e2 +'",'+
              '"' + h3 +'":"' + e3 +'"}}}';
              var obj = JSON.parse(ugly);
              var pretty = JSON.stringify(obj, undefined, 4);
              document.getElementById('myTextArea').value = pretty;
          }
      function showSaved() {

              var obj = JSON.parse(document.getElementById('savedJSON').innerHTML);
              var showJSON = JSON.stringify(obj, undefined, 4);
              document.getElementById('saved').value = showJSON;
        }

        function update() {
          var select = document.getElementById('networks');
          var option = select.options[select.selectedIndex];
          document.getElementById('wifiname').value = option.value;
        }
          window.onload = showSaved;
  </script>)rawliteral";

void blink_led(int blink_count, int led_delay,  bool onExit){
    digitalWrite(output, LOW);
    delay(led_delay);
    for(int i=0; i<blink_count;i++){
        digitalWrite(output, !digitalRead(output));
        delay(led_delay);
    }
    if(onExit){
        digitalWrite(output, HIGH);
    }
    else{
        digitalWrite(output, LOW);
    }
}

String wificredits(const String& var){
    //Serial.println(var);
    if(var == "BUTTONPLACEHOLDER"){
        String buttons = "";
        buttons += "<div class=\"main-container\">";
        buttons += "<label class=\"label\" for=\"wifiname\">WIFI name</label>";
        buttons += "<input class=\"control\" type=\"text\" id=\"wifiname\" value=\""+inputSSID+"\"></div>";

        buttons += "<div class=\"main-container\">";
        buttons += "<label class=\"label\" for=\"wifipass\">WIFI password</label>";
        buttons += "<input class=\"control\" type=\"password\" id=\"wifipass\" value=\""+inputPASS+"\"></div>";
        buttons += "<label style=\"display:none;\" id=\"savedJSON\">" + jsonStr + "</label>";
        buttons+=wifilist;

        return buttons;
    }
    return String();
}

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

void notFound(AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Not found");
}

void saveConfig(){
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
}

void setup() {
// write your initialization code here
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
        blink_led(15,50,true);
        digitalWrite(output, HIGH);
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
        saveConfig();
        Serial.println("send request...");
        request->send(200, "text/html", "done");
        //request->redirect("/save");
    });

    server.on("/getID", HTTP_POST, [](AsyncWebServerRequest *request){
        Serial.println("recive ID POST");
        Serial.println("send ID via post request...");
        request->send(200, "text/html", WiFi.hostname().c_str());
        blinkID=true;
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
    server.onNotFound(notFound);
    server.begin();

}

void loop() {
// write your code here


    if (blinkID){
        digitalWrite(outputGreen, LOW);
        blink_led(5,500,false);
        digitalWrite(outputGreen, HIGH);
        blinkID=false;
    }

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
            blink_led(5,100,true);
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

            blink_led(10,100,false);
            digitalWrite(outputGreen, HIGH);
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