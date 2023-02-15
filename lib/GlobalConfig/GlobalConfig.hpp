//
// Created by rmaks on 15.02.2023.
//

#ifndef EVENT_BUTTON_GLOBALCONFIG_HPP
#define EVENT_BUTTON_GLOBALCONFIG_HPP

#include <Arduino.h>



//old
extern const int output = 4;
extern const int outputGreen = 13;  //5
extern const int buttonPin = 5;  //2


//DNS SETUP
extern const byte DNS_PORT = 53;

extern bool restart=false;
extern bool onConfig=false;


extern String qsid="";
extern String qpass="";
extern String qeventhost="";
extern String qevents="";
extern String spiff_cont ="";
extern String inputSSID ="";
extern String inputPASS ="";
extern String wifilist="";
extern String jsonStr="";

extern String postMessage;
extern char host[] = "BUTTON protorype 2";

extern const char* PARAM_INPUT = "inputdata";


extern String wificredits(const String& var){
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



#endif //EVENT_BUTTON_GLOBALCONFIG_HPP
