//
// Created by rmaks on 01-Apr-24.
//

#ifndef EVENT_BUTTON_EXTERNALINTERFACESERVICE_H
#define EVENT_BUTTON_EXTERNALINTERFACESERVICE_H

#include <Arduino.h>
#include <map>
#include <ESPAsyncWebServer.h>

typedef enum {
    EXTERNAL_LED,
    EXTERNAL_BEEP
} external_trigger;

class ExternalInterfaceService {

private:
    typedef std::map<String, bool> PropertyTrigger;
    typedef std::map<external_trigger, PropertyTrigger> OptionsSet;
    typedef std::map<external_trigger, bool&> OptionsToHandle;
    typedef std::map<external_trigger, String> OptionsDef;

    PropertyTrigger switchTrigger = {
            {"on", true},
            {"off", false}
    };

    OptionsSet options = {
            {EXTERNAL_LED, switchTrigger},
            {EXTERNAL_BEEP, switchTrigger}
    };

    OptionsDef optionsDef = {
            {EXTERNAL_LED, "led"},
            {EXTERNAL_BEEP, "beep"}
    };

public:
    void handleTriggers(AsyncWebServerRequest *request, OptionsToHandle optionsToSwitch);

};


#endif //EVENT_BUTTON_EXTERNALINTERFACESERVICE_H
