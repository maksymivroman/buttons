//
// Created by rmaks on 27.02.2023.
//

#ifndef EVENT_BUTTON_HTMLCOMPONENTBUILDER_H
#define EVENT_BUTTON_HTMLCOMPONENTBUILDER_H

#include <Arduino.h>
#include "GlobalConfig.hpp"

class HTMLComponentBuilder {
public:
    String componentById(const String& ref);
    void setHtmlPageData(String ssid, String password, String eventsConfig, NETWORKLIST list);
private:
    String events, networkSsid, networkPassword;
    NETWORKLIST networkList;
    String wiFiList();
};


#endif //EVENT_BUTTON_HTMLCOMPONENTBUILDER_H
