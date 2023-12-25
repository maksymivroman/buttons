//
// Created by rmaks on 07.03.2023.
//

#ifndef EVENT_BUTTON_EVENTSSERVICE_H
#define EVENT_BUTTON_EVENTSSERVICE_H

#include <Arduino.h>
#include "TelegramIntegration/TelegramIntegration.h"

class EventsService {

public:
    void SendEvents();
    void SetEvents(String eventsData);
    void UpdateEventProp(String prop);
    void SendEventsOnKeystoreChange();

    TelegramIntegration *telegramBotRef = nullptr;

private:
    void SendMessageToTelegram(String message);
    void SendHttpEvent(String &host, String &payload);
    void UpdateEventDataWithKeystore(String &eventData, String &replaceWith);
    void ProcessToSend(boolean onlyFromKeystore = false);

    String events;

    String keystoreProp;
    const char *propPattern = "$prop$";

};


#endif //EVENT_BUTTON_EVENTSSERVICE_H
