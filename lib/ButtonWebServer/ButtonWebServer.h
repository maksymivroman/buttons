//
// Created by rmaks on 15.02.2023.
//

#ifndef EVENT_BUTTON_BUTTONWEBSERVER_H
#define EVENT_BUTTON_BUTTONWEBSERVER_H


#include "SettingsService.h"

class ButtonWebServer {
protected:
//    AsyncWebServer _server;

public:
    void startServer();
    bool requireRestart = false;

private:
    String wiFiList;
//    String wiFiCredits();
//    static void notFound(AsyncWebServerRequest *request);
//    void saveConfig();
    static String wiFiCredits(const String &var);

    SettingsService ButtonSettings;
};


#endif //EVENT_BUTTON_BUTTONWEBSERVER_H
