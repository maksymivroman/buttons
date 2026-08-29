//
// Created by rmaks on 27.02.2023.
//

#ifndef EVENT_BUTTON_HTMLCOMPONENTBUILDER_H
#define EVENT_BUTTON_HTMLCOMPONENTBUILDER_H

#include <Arduino.h>

class HTMLComponentBuilder {
public:
    String componentById(const String &ref);

    void setClientMode(bool isClient);

private:
    bool isClientMode{false};
};

#endif //EVENT_BUTTON_HTMLCOMPONENTBUILDER_H
