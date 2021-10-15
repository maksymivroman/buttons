  #include <ESP8266WiFi.h>
  #include <ESPAsyncTCP.h>
  #include <ESPAsyncWebServer.h>
  #include <ESP8266HTTPClient.h>
  #include "ArduinoJson.h"
  #include <EEPROM.h>
  #include "FS.h"
  
//const char* ssid     = "[MikroTik]";
//const char* password = "19591983";

const int output = 4;  //5
const int buttonPin = 5;  //2

// Variables will change:
int ledState = LOW;          
int buttonState;             
int lastButtonState = LOW;

bool restart;

      String qsid="";
      String qpass="";
      String qeventhost="";
      String qevents="";
      String spiff_cont ="";

unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers

AsyncWebServer server(80);
const char* PARAM_INPUT = "inputdata";

// HTML
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>button config page</title>
    <style>
        html{ color: #333333; font-size: 6px; font-family: Arial }
        .container{ background: #949393; display: flex; flex-direction: column; align-items: center;}
        .main{ color: #333333; display: flex; flex-direction: column; align-items: center; justify-content:center;}
        .main-container{display: flex; flex-direction: column; align-items: center; justify-content: right; margin: 2rem;}
        label{font-size: 3rem; color: white}
        .container>h1{ font-size: 5rem;}
        button{font-size: 3rem; color: chocolate;}
        .wifi-creditaince{ display: flex; flex-direction: row; justify-content: center; }
        input{font-size: 3rem}
    </style>

</head>
<body class="container">
    <h1>button configuration page</h1>
    <div class="main">
        <div class="wifi-creditaince">
            <div class="main-container">
                <label for="wifiname">WIFI name</label>
                <input type="text" id="wifiname" value="X[R]">
            </div>

            <div class="main-container">
                <label for="wifipass">WIFI password</label>
                <input type="password" id="wifipass" value="1133557799">
            </div>
        </div>

        <div class="wifi-creditaince">
            <div class="main-container">
                <label for="wifiname">HOSTS</label>
                <input type="text" id="host1" value="https://webhook.site/71c2f7ef-1dff-492b-9377-5757859cb3c4">
                <input type="text" id="host2" value="http://192.168.2.71:55554/event">
                <input type="text" id="host3" value="http:2">
            </div>

            <div class="main-container">
                <label for="wifipass">EVENTS</label>
                <input type="text" id="event1" value="{\"eventName1\":\"DYNAMIC_EVENT1\"}">
                <input type="text" id="event2" value="{\"eventName2\":\"DYNAMIC_EVENT2\"}">
                <input type="text" id="event3" value="{\"eventName3\":\"DYNAMIC_EVENT3\"}">
            </div>

        </div>


    </div>
    <div class="main-container">
        <button type="button" onclick="mergecontent()">save and reboot</button>
    </div>
</body>



</html>

<script>
     function mergecontent(){
         var wifiname = document.getElementById('wifiname').value;
         var wifipass = document.getElementById('wifipass').value;
         var h1 = document.getElementById('host1').value;
         var h2 = document.getElementById('host2').value;
         var h3 = document.getElementById('host3').value;
         var e1 = document.getElementById('event1').value;
         var e2 = document.getElementById('event2').value;
         var e3 = document.getElementById('event3').value;

         let data='inputdata={ "inputdata" :{"wifiname":"' + wifiname + '","wifipass":"'+ wifipass +'","eventdata": {"'+
             h1 +'":"' + e1 +'",'+
             '"' + h2 +'":"'  + e2 +'",'+
             '"' + h3 +'":"' + e3 +'"}}}';

        window.location.href = '/get?'+data;
        //console.log(data);
    }
</script>)rawliteral";

void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}
      
void setup(){
//    uint8_t newMACAddress[] = {0x30, 0xAE, 0xA4, 0x07, 0x0D, 0x66}; 
//  String newHostname = "button01";
//
//  wifi_set_macaddr(STATION_IF, &newMACAddress[0]);
//   WiFi.hostname(newHostname.c_str());
//  WiFi.mode(WIFI_STA); 

  
  
  pinMode(output, OUTPUT);
    digitalWrite(output, LOW);
  pinMode(buttonPin, INPUT);

  delay(2000);
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
  StaticJsonDocument<600> eep;
  deserializeJson(eep, spiff_cont);
  JsonObject data = eep["inputdata"];
        Serial.println("Json// {inputdata}:");
    const char* wifiname = data["wifiname"];
    const char* wifipass = data["wifipass"];
    const char* eventdata = data["eventdata"];
        Serial.println(data.size());

  StaticJsonDocument<600> spif;
  deserializeJson(spif, spiff_cont);
  JsonObject evdata = spif["inputdata"]["eventdata"];
    Serial.println("Json// {inputdata}:");
        String jsonStr;
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
      esid += char(EEPROM.read(i));
    }
  Serial.println();
  Serial.print("SSID: ");
  Serial.println(esid);
  Serial.println("Reading EEPROM pass");
 
  String epass = "";
    for (int i = 32; i < 96; ++i)
    {
      epass += char(EEPROM.read(i));
    }
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
          for(int i=0; i!=10; i++) {
              digitalWrite(output, HIGH);
              delay(200);
              digitalWrite(output, LOW);
              delay(200);
          }

          digitalWrite(output, HIGH);
          Serial.println(">>>CONFIG MODE<<<");
          Serial.println("Setting soft-AP ... ");
          WiFi.softAP("BUTTON_CONFIG", "1133557799");
            IPAddress IP = WiFi.softAPIP();
            Serial.print("AP IP address: ");
            Serial.println(IP);          
        }    
    else{  
          // Connect to Wi-Fi
          Serial.printf("hostname: %s\n", WiFi.hostname().c_str());
            WiFi.begin(esid, epass);
              while (WiFi.status() != WL_CONNECTED) {
                delay(1000);
                Serial.println("Connecting to WiFi..");
                digitalWrite(output,!digitalRead(output));
              }
          digitalWrite(output, HIGH); 
        } 
  // Print ESP Local IP Address
 Serial.println(WiFi.localIP());  
  // Route for root / web page
 server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
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

            StaticJsonDocument<600> doc_eeprom;
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
                            // Serial.println(qeventhost);
                            // Serial.println("");
                            // Serial.println(qevents);
                            // Serial.println("");

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
                    // Serial.println("writing eeprom qeventhost:");
                    // for (int i = 0; i < qeventhost.length(); ++i)
                    // {
                    //   EEPROM.write(96 + i, qeventhost[i]);
                    //   Serial.print("Wrote: ");
                    //   Serial.println(qeventhost[i]);
                    // }

                    //   Serial.println("writing eeprom events:");
                    // for (int i = 0; i < qevents.length(); ++i)
                    // {
                    //   EEPROM.write(224 + i, qevents[i]);
                    //   Serial.print("Wrote: ");
                    //   Serial.println(qevents[i]);
                    // }
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
    
////parce JSON  
//    String stri ;
//int str_len = inputMessage.length() + 1; 
//char char_array[str_len];
//stri.toCharArray(char_array, str_len);

  
  
    Serial.println(inputMessage);
    request->send(200, "text/html", "button will reset now with parameters:(" 
                                     + inputParam + ") with value: " + inputMessage );

    Serial.println("restart ESP...");
    delay(1000);
    restart=true;
    
  });
  server.onNotFound(notFound);
  server.begin();
}  


void loop() {
 
if (restart){
    ESP.reset();
  }

int reading = digitalRead(buttonPin);

if (reading !=0) {
  Serial.println("btn");
            for(int i=0; i!=10; i++) {
              digitalWrite(output, LOW);
              delay(100);
              digitalWrite(output, HIGH);
              delay(100);
          }

      StaticJsonDocument<600> main;
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
          int httpCode = http.POST(postname);                 
          Serial.println(httpCode);                    
          http.end(); 
    }
}

    digitalWrite(output, HIGH);
    delay(3000);
    digitalWrite(output, LOW);
    
    
 

//
//            //

            //
                  //std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
                  //client->setInsecure();
                  
 
            //

  }


}