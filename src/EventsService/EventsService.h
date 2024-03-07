//
// Created by rmaks on 07.03.2023.
//

#ifndef EVENT_BUTTON_EVENTSSERVICE_H
#define EVENT_BUTTON_EVENTSSERVICE_H

#include <Arduino.h>

class EventsService {

public:
    void SendEvents();
    void SetEvents(String eventsData);
private:
    void SendHttpEvent(String &host, String &payload);
    void ProcessToSend();

    String events;

};


#endif //EVENT_BUTTON_EVENTSSERVICE_H
