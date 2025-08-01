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

extern NetworkService networkService;

typedef std::map<EVENT_TRIGGER, std::array<char, 3>> TriggersMap;

class EventsService {

public:
    void SendEvents(EVENT_TRIGGER triggeredBy = DEFAULT_TRIGGER);
    void SetEvents(String eventsData, bool serialEnabled = true);
    void SendEventsOnKeystoreChange();

private:
    void SendHttpEvent(String &host, String &payload);
    void SendSerialEvent(String &payload);
    void ProcessEvents(EVENT_TRIGGER triggeredBy);
    void ProcessToSend(String &host, String &payload);
    bool isEventMatchTrigger(EVENT_TRIGGER trigger, const String& eventRequestHost) const;

    bool isDefaultEvent(const String& eventRequestHost);

    String normalizeRequestHost(String host, EVENT_TRIGGER trigger);

    String events;
    bool _serialEnabled;

    TriggersMap _triggers{
            {DEFAULT_TRIGGER, {}},
            {KEYSTORE_UPDATE, {'$', 'K', '$'}},
            {STATE_RELEASED, {'$', 'P', '$'}},
            {STATE_PRESSED, {'$', 'R', '$'}},
    };

};


#endif //EVENT_BUTTON_EVENTSSERVICE_H
