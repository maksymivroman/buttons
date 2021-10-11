  #include <ESP8266WiFi.h>
  #include <ESPAsyncTCP.h>
  #include <ESPAsyncWebServer.h>
  #include <ESP8266HTTPClient.h>
  #include "ArduinoJson.h"
  #include <EEPROM.h>
  
//const char* ssid     = "[MikroTik]";
//const char* password = "19591983";



const char* PARAM_INPUT_1 = "state";

const int output = 4;  //5
const int buttonPin = 5;  //2

// Variables will change:
int ledState = LOW;          
int buttonState;             
int lastButtonState = LOW; 

      String qsid="";
      String qpass="";
      String qeventhost="";
      String qevents="";
      
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers

AsyncWebServer server(80);

const char* PARAM_INPUT = "inputdata";

// HTML web page to handle 3 input fields (input1, input2, input3)
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>button config page</title>
    <style>
        html{ color: #333333; font-size: 8px; font-family: Arial }
        .container{ color: #333333; display: flex; flex-direction: column; align-items: center;}
        .main{ color: #333333; display: flex; flex-direction: column; align-items: center; justify-content:center;}
        .main-container{display: flex; flex-direction: column; align-items: center; justify-content: right; margin-top: 4rem;}
        label{font-size: 3rem;}
        .container>h1{ font-size: 5rem;}
        button{font-size: 3rem; color: chocolate; }
    </style>

</head>
<body class="container">
    <h1>button configuration page</h1>
    <div class="main">
        <div class="main-container">
            <label for="wifiname">WIFI name</label>
            <input type="text" id="wifiname">
        </div>

        <div class="main-container">
            <label for="wifipass">WIFI password</label>
            <input type="password" id="wifipass">
        </div>
        
        <div class="main-container">
            <label for="events">event host</label>
            <input type="text" id="event-host">
        </div>
        
        <div class="main-container">
            <label for="events">event content</label>
            <input type="text" id="events">
        </div>
    </div>
    <div class="main-container">
        <button type="button" onclick="mergecontent()">save and reboot</button>
    </div>
</body>



</html>

<script>
     function mergecontent(){
        var input1 = document.getElementById('wifiname').value;
        var input2 = document.getElementById('wifipass').value;
        var input3 = document.getElementById('event-host').value;
        var input4 = document.getElementById('events').value;
         let text = "inputdata=" + '{ "inputdata" : ' +
             '{ "wifiname":'+'"' + input1 +'"' +' , '+
             ' "wifipass":'+'"' + input2 +'"'+' , '+
             ' "eventhost":'+'"' + input3 +'"'+' , '+
             ' "events":'+'"'+ input4 +'"'+'   } }';
         window.location.href = '/get?'+text;
    }
</script>)rawliteral";

void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}



      
void setup(){
  delay(2000);
  Serial.begin(115200);
   
  EEPROM.begin(1024);
 
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

  String ehost = "";
  for (int i = 96; i < 224; ++i)
  {
    ehost += char(EEPROM.read(i));
  }
  Serial.print("HOST: ");
  Serial.println(ehost);

  String eevent = "";
  for (int i = 224; i < 512; ++i)
  {
    eevent += char(EEPROM.read(i));
  }
  Serial.print("EVENT: ");
  Serial.println(eevent);


       qeventhost = ehost;
       qevents = eevent;
       
  /////////////////////
  pinMode(output, OUTPUT);
  digitalWrite(output, LOW);
  pinMode(buttonPin, INPUT);

  //==========if btn is presed then wifi hotspot is active untill reboot
  
    int reading = digitalRead(buttonPin);
    if (reading ==0){
          for(int i=0; i!=10; i++) {
              digitalWrite(output, HIGH);
              delay(200);
              digitalWrite(output, LOW);
              delay(200);
          }
          digitalWrite(output, HIGH);
          Serial.print("Setting soft-AP ... ");
          WiFi.softAP("BUTTON_CONFIG", "1133557799");
            IPAddress IP = WiFi.softAPIP();
            Serial.print("AP IP address: ");
            Serial.println(IP);
//          while (WiFi.status() != WL_CONNECTED) {
//            delay(500);
//            Serial.print(".");
//        }
          
  }
    
    else{ 
 
  // Connect to Wi-Fi

String newHostname = "button01";
WiFi.mode(WIFI_STA);

WiFi.hostname(newHostname.c_str());

Serial.printf("New hostname: %s\n", WiFi.hostname().c_str());
  
  WiFi.begin(esid, epass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
    digitalWrite(output,!digitalRead(output));
  }
 digitalWrite(output, HIGH); 
 }
 //==========
 
  // Print ESP Local IP Address
  Serial.println(WiFi.localIP());
  
  // Route for root / web page
 server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });

  // Send a GET request to <ESP_IP>/get?input1=<inputMessage>
  server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request) {

    String inputMessage;
    String inputParam;
    // GET input1 value on <ESP_IP>/get?input1=<inputMessage>
    if (request->hasParam(PARAM_INPUT)) {
      inputMessage = request->getParam(PARAM_INPUT)->value();
      inputParam = PARAM_INPUT;}

      
    else {
      inputMessage = "No message sent";
      inputParam = "none";
    }
    
    ////parce JSON
      StaticJsonDocument<600> doc;
//    String stri ;
//
//// Length (with one extra character for the null terminator)
//int str_len = inputMessage.length() + 1; 
//
//// Prepare the character array (the buffer) 
//char char_array[str_len];
//
//// Copy it over 
//stri.toCharArray(char_array, str_len);


  deserializeJson(doc, inputMessage);

  //char json[] =inputMessage;
  JsonObject data = doc["inputdata"];
  Serial.println(data);
  const char* wifiname = data["wifiname"];
  const char* wifipass = data["wifipass"];
  const char* eventhost = data["eventhost"];
  const char* events = data["events"];
Serial.println(events);
  /////////////////EEPROM write////////////////////////////////
//      String qsid = wifiname;
//      String qpass = wifipass;
//      String qeventhost = eventhost;
//      String qevents = events;
       qsid = wifiname;
       qpass = wifipass;
       qeventhost = eventhost;
       qevents = events;
      if (qsid.length() > 0 && qpass.length() > 0) {
        Serial.println("clearing eeprom");
        for (int i = 0; i < 1024; ++i) {
          EEPROM.write(i, 0);
        }
                Serial.println(qsid);
                Serial.println("");
                Serial.println(qpass);
                Serial.println("");
                Serial.println(qeventhost);
                Serial.println("");
                Serial.println(qevents);
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

        Serial.println("writing eeprom qeventhost:");
        for (int i = 0; i < qeventhost.length(); ++i)
        {
          EEPROM.write(96 + i, qeventhost[i]);
          Serial.print("Wrote: ");
          Serial.println(qeventhost[i]);
        }

          Serial.println("writing eeprom events:");
        for (int i = 0; i < qevents.length(); ++i)
        {
          EEPROM.write(224 + i, qevents[i]);
          Serial.print("Wrote: ");
          Serial.println(qevents[i]);
        }
        EEPROM.commit();
        };
  ////////////////////
  // Print values.
  Serial.println(wifiname);
  Serial.println(wifipass);
  Serial.println(events);
  //////end JSON
  
    Serial.println(inputMessage);
    request->send(200, "text/html", "button will reset now with parameters:(" 
                                     + inputParam + ") with value: " + inputMessage );

    Serial.print("restart ESP...");
    delay(1000);
    ESP.reset();
  });
  server.onNotFound(notFound);
  server.begin();
}  
void loop() {
 
int reading = digitalRead(buttonPin);

if (reading ==0) {
  Serial.println("btn");
            for(int i=0; i!=10; i++) {
              digitalWrite(output, LOW);
              delay(100);
              digitalWrite(output, HIGH);
              delay(100);
          }
          
               //

                  char Buf[128];
                  qeventhost.toCharArray(Buf, 128);
                  char evBuf[128];
                  qevents.toCharArray(evBuf, 128);
    
    
//                  std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
//                  client->setInsecure();
//                  
//                    //WiFiClient client;
//                    HTTPClient http;
//                      Serial.println(qevents + "--HOST-:" + Buf);                  
//                    http.begin(*client, Buf);           //////qeventhost or "https://webhook.site/71c2f7ef-1dff-492b-9377-5757859cb3c4"
//                    http.addHeader("Content-Type", "application/json");                  
//                    int httpCode = http.POST(qevents);    ///////// "{\"eventName\":\"DYNAMIC_EVENT\"}"             
//                    Serial.println(httpCode);                    
//                    http.end();  
//                      digitalWrite(output, HIGH);
//                      delay(3000);
//                      digitalWrite(output, LOW);
//
//            //

            //
                  //std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
                  //client->setInsecure();
                  
                    WiFiClient client;
                    HTTPClient http;
                    
                    Serial.println(qevents + "--HOST-:" + Buf);                  
                    http.begin(client, Buf); //http://192.168.2.71:55554/event
                    http.addHeader("Content-Type", "application/json");                  
                    int httpCode = http.POST(evBuf);                 
                    Serial.println(httpCode);                    
                    http.end();  
            //

  }

  // If the switch changed, due to noise or pressing:
  if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != buttonState) {
      buttonState = reading;

      // only toggle the LED if the new button state is HIGH
      if (buttonState == HIGH) {
        ledState = !ledState;
      }
    }
  }

  lastButtonState = reading;
}