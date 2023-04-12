//
// Created by rmaks on 27.02.2023.
//

#include "HTMLComponentBuilder.h"

void HTMLComponentBuilder::setHtmlPageData(String ssid, String password, String eventsConfig, NETWORKLIST list, EEPROMSETTINGS config) {
    networkSsid = ssid;
    networkPassword = password;
    events = eventsConfig;
    networkList = list;
    configuration = config;
}

String HTMLComponentBuilder::componentById(const String &ref) {
    String data = "";
    if (ref == "NETWORKINFO") {
        data += "<div class=\"main-container\">";
        data += R"(<label class="label" for="wifiname">WiFi SSID</label>)";
        data += R"(<input class="control" type="text" id="wifiname" value=")" + networkSsid + "\"></div>";
        data += "<div class=\"main-container\">";
        data += R"(<label class="label" for="wifipass">WiFi password</label>)";
        data += R"(<input class="control" type="password" id="wifipass" value=")" + networkPassword + "\"></div>";
        return data;
    } else if (ref == "EVENTINFO") {
        data += R"(<label style="display:none;" id="savedJSON">)" + events + "</label>";
        return data;
    } else if (ref == "WIFILIST") {
        data += wiFiList();
        return data;
    }else if (ref == "CONFIGURATION") {
        data += createConfigurationObject(configuration);
        return data;
    } else if (ref == "FWVERSION") {
        data += __DATE__;
        return data;
    } else if (ref == "MAC") {
        data += WiFi.macAddress();
        return data;
    } else if (ref == "HEAP") {
        data += ESP.getFreeHeap();
        return data;
    } else if (ref == "IP") {
        data += WiFi.localIP().toString();
        return data;
    }
    return ref;
}

String HTMLComponentBuilder::wiFiList() {
    String wifilist = "";
    wifilist = "<div class=\"main-container\">";
    wifilist += "<label class=\"label\" for=\"wifiname\">WIFI List</label>";
    wifilist += "<select class=\"control\" id=\"networks\" onChange=\"update()\">";
    Serial.print("Scan start ... ");
    int n = networkList.size;
    Serial.print(n);
    Serial.println(" network(s) found");
    for (int i = 0; i < n; i++) {
        Serial.println(networkList.arr[i]);
        wifilist += "<option value=\"" + networkList.arr[i] + "\">" + networkList.arr[i] + "</option>";
    }
    wifilist += "</select></div>";
    return wifilist;
}

String HTMLComponentBuilder::createConfigurationObject(EEPROMSETTINGS data) {
    String configObj = R"({
            serialEnabled: <serialEnabled>,
            clientWebAccess: <clientWebAccess>,
            enableOtaUpdate: <enableOtaUpdate>,
            useDnsName: <useDnsName>,
            useSound: <useSound>,
            fwVersion: <fwVersion>
        })";

    configObj.replace("<serialEnabled>", String(data.serialEnabled));
    configObj.replace("<clientWebAccess>", String(data.clientWebAccess));
    configObj.replace("<enableOtaUpdate>", String(data.enableOtaUpdate));
    configObj.replace("<useDnsName>", String(data.useDnsName));
    configObj.replace("<useSound>", String(data.useSound));
    configObj.replace("<fwVersion>", String(data.fwVersion));

    return configObj;
}

