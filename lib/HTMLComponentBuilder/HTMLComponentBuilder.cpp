//
// Created by rmaks on 27.02.2023.
//

#include "HTMLComponentBuilder.h"

void HTMLComponentBuilder::setHtmlPageData(String ssid, String password, String eventsConfig, NETWORKLIST list, EEPROMSETTINGS config, INTEGRATIONSETTINGS integrationData, bool isClient) {
    networkSsid = ssid;
    networkPassword = password;
    events = eventsConfig;
    networkList = list;
    configuration = config;
    integrationSettings = integrationData;
    isClientMode = isClient;
}

String HTMLComponentBuilder::componentById(const String &ref) {
    String data = "";
    if (ref == "NETWORKINFO") {
        data += "<div class=\"main-container\">";
        data += R"(<label for="wifiname">WiFi SSID</label>)";
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
    } else if (ref == "CLIENTMODEOPTIONS") {
        if (!isClientMode){
            data += R"(<div class="item">
                <input type="checkbox" id="enableOtaUpdate" name="otaUpdate">
                <label style="margin-left: 8px;" for="enableOtaUpdate">Firmware Update on Wi-Fi client mode</label>
            </div>)";
            data += R"(<div class="item">
                <input type="checkbox" id="clientWebAccess" name="hotspotAccess">
                <label style="margin-left: 8px;" for="clientWebAccess">web access on Wi-Fi client mode</label>
            </div>)";
        }
        return data;
    } else if (ref == "CONFIGURATION") {
        data += createConfigurationObject(configuration);
        return data;
    } else if (ref == "INTEGRATION") {
        data += createIntegrationDataObject(integrationSettings);
        return data;
    } else if (ref == "FWVERSION") {
        data += currentFirmwareVersion;
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
    wifilist += "<label for=\"networks\">Select available network</label>";
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
            customHSsid: <useCustomHSsid>,
            useTelegramIntegration: <useTelegramIntegration>,
            fwVersion: <fwVersion>,
            hotspotSsid: "<hotspotSsid>"
        })";

    configObj.replace("<serialEnabled>", String(data.serialEnabled));
    configObj.replace("<clientWebAccess>", String(data.clientWebAccess));
    configObj.replace("<enableOtaUpdate>", String(data.enableOtaUpdate));
    configObj.replace("<useDnsName>", String(data.useDnsName));
    configObj.replace("<useSound>", String(data.useSound));
    configObj.replace("<useTelegramIntegration>", String(data.useTelegramIntegration));
    configObj.replace("<fwVersion>", String(data.fwVersion));
    configObj.replace("<hotspotSsid>", String(data.hotspotSsid));
    configObj.replace("<useCustomHSsid>", String(data.useCustomHSsid));

    return configObj;
}

String HTMLComponentBuilder::createIntegrationDataObject(INTEGRATIONSETTINGS data) {
    String configObj = R"({
            tToken: "<tToken>",
            tChanelID: <tChanelID>,
            tPrefix: "<tPrefix>",
            tSuffix: "<tSuffix>"
        })";
    configObj.replace("<tToken>", String(data.tToken));
    configObj.replace("<tChanelID>", String(data.tChanelID));
    configObj.replace("<tPrefix>", String(data.tPrefix));
    configObj.replace("<tSuffix>", String(data.tSuffix));

    return configObj;
}
