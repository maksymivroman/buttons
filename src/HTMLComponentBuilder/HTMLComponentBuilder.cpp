//
// Created by rmaks on 27.02.2023.
//

#include "HTMLComponentBuilder.h"

void HTMLComponentBuilder::setHtmlPageData(String ssid, String password, String eventsConfig, NETWORKLIST list, EEPROM_SETTINGS config, INTEGRATIONSETTINGS integrationData, bool isClient) {
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
        data += R"(<input class="control mt-m" type="text" id="wifiname" list="wifiList" value=")" + networkSsid + "\"></div>";
        data += "<div class=\"main-container\">";
        data += R"(<label for="wifipass">WiFi password</label>)";
        data += R"(<input class="control mt-m" type="password" id="wifipass" value=")" + networkPassword + "\"></div>";
        return data;
    } else if (ref == "EVENTINFO") {
        data += events;
        return data;
    } else if (ref == "WIFILIST") {
        data += wiFiList();
        return data;
    } else if (ref == "CLIENTMODEOPTIONS") {
        if (!isClientMode){
            data += R"(<div class="item">
                <input type="checkbox" id="enableOtaUpdate" name="otaUpdate">
                <label style="margin-left: 8px;" for="enableOtaUpdate">Firmware Update on client mode</label>
            </div>)";
            data += R"(<div class="item">
                <input type="checkbox" id="clientWebAccess" name="hotspotAccess">
                <label style="margin-left: 8px;" for="clientWebAccess">Web access on client mode</label>
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
        data += currentFWVersion.str_version();
        return data;
    } else if (ref == "MAC") {
        data += WiFi.macAddress();
        return data;
    } else if (ref == "HEAP") {
        data += ESP.getFreeHeap();
        return data;
    } else if (ref == "KSKEYS") {
        data += keystore.currentItemsCount();
        return data;
    } else if (ref == "TOGGLESTATE") {
        data += buttonState.getToggleMode() ? "PRESSED" : "RELEASED";
        return data;
    } else if (ref == "IP") {
        switch (WiFi.getMode()) {
            case WIFI_STA:
                data += WiFi.localIP().toString();
                break;
            case WIFI_AP_STA:
                data += WiFi.softAPIP().toString();
                break;
            default:
                data += "unknown";
                break;
        }
        return data;
    }
    return ref;
}

String HTMLComponentBuilder::wiFiList() {
    String wifilist = "";
    const uint8_t networksFund = networkList.size;
    wifilist += "<datalist id=\"wifiList\">";
    for (int i = 0; i < networksFund; i++) {
        wifilist += "<option value=\"" + networkList.arr[i] + "\"></option>";
    }
    wifilist += "</datalist>";
    return wifilist;
}

String HTMLComponentBuilder::createConfigurationObject(EEPROM_SETTINGS data) {
    String configObj = R"({
            loggerEnabled: <loggerEnabled>,
            loggerLevel: <loggerLevel>,
            clientWebAccess: <clientWebAccess>,
            enableOtaUpdate: <enableOtaUpdate>,
            useDnsName: <useDnsName>,
            useSound: <useSound>,
            customHSsid: <useCustomHSsid>,
            useTelegramIntegration: <useTelegramIntegration>,
            remoteTriggering:<remoteTriggering>,
            fwVersion: <fwVersion>,
            hotspotSsid: "<hotspotSsid>",
            keystoreEnabled: <keystoreEnabled>,
            sendEventOnKeystoreUpdate: <sendEventOnKeystoreUpdate>,
            delaySendEvents: <delaySendEvents>,
            statisticEnabled: <statisticEnabled>,
            statisticLevel: <statisticLevel>,
            statisticApi: "<statisticApi>",
            remoteStateChange: <remoteStateChange>,
            saveLastState: <saveLastState>,
            restoreLastStateOnLoad: <restoreLastStateOnLoad>
        })";

    configObj.replace("<loggerEnabled>", String(data.loggerEnabled));
    configObj.replace("<loggerLevel>", String(data.loggerLevel));
    configObj.replace("<clientWebAccess>", String(data.clientWebAccess));
    configObj.replace("<enableOtaUpdate>", String(data.enableOtaUpdate));
    configObj.replace("<useDnsName>", String(data.useDnsName));
    configObj.replace("<useSound>", String(data.useSound));
    configObj.replace("<useTelegramIntegration>", String(data.useTelegramIntegration));
    configObj.replace("<remoteTriggering>", String(data.remoteTriggering));
    configObj.replace("<fwVersion>", String(data.fwVersion));
    configObj.replace("<hotspotSsid>", String(data.hotspotSsid));
    configObj.replace("<useCustomHSsid>", String(data.useCustomHSsid));
    configObj.replace("<keystoreEnabled>", String(data.keystoreEnabled));
    configObj.replace("<sendEventOnKeystoreUpdate>", String(data.sendEventOnKeystoreUpdate));
    configObj.replace("<delaySendEvents>", String(data.delaySendEvents));
    configObj.replace("<statisticEnabled>", String(data.statisticEnabled));
    configObj.replace("<statisticLevel>", String(data.statisticLevel));
    configObj.replace("<statisticApi>", String(data.statisticApi));
    configObj.replace("<remoteStateChange>", String(data.remoteStateChange));
    configObj.replace("<saveLastState>", String(data.saveLastState));
    configObj.replace("<restoreLastStateOnLoad>", String(data.restoreLastStateOnLoad));

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
