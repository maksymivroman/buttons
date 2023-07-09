//
// Created by rmaks on 07.03.2023.
//

#ifndef EVENT_BUTTON_EVENTSSERVICE_H
#define EVENT_BUTTON_EVENTSSERVICE_H

#include <Arduino.h>

#include "TelegramIntegration.h"

class EventsService {

public:
    void SendEvents();
    void SetEvents(String eventsData);

    TelegramIntegration *telegramBotRef = nullptr;

private:
    void SendMessageToTelegram(String message);
    void SendHttpEvent(String &host, String &payload);

    String events;

};


#endif //EVENT_BUTTON_EVENTSSERVICE_H
