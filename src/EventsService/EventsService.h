//
// Created by rmaks on 07.03.2023.
//

#ifndef EVENT_BUTTON_EVENTSSERVICE_H
#define EVENT_BUTTON_EVENTSSERVICE_H

#include <Arduino.h>
#include <map>
#include <array>
#include "Global/Global.hpp"
#include "NetworkService/NetworkService.h"
#include "ArduinoJson.h"

extern NetworkService networkService;


typedef enum ACTION_RESULT {
    SUCCESS, IGNORED, FAILED
} ActionResult;

class EventsService {

public:
    void SetEvents(String eventsData, bool serialEnabled = true);
    void performAction(ACTION_TRIGGER actionTrigger);

private:
    void executeActions(const JsonObject &event);
    ActionResult httpRequest(const String &host = "", const String &payload = "");
    ActionResult serial(const String &payload = "");

    /** @warning Not implemented*/
    ActionResult io() { return IGNORED; };

    String events;
    bool _serialEnabled;

};


#endif //EVENT_BUTTON_EVENTSSERVICE_H
