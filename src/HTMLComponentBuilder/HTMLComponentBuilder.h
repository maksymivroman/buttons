//
// Created by rmaks on 27.02.2023.
//

#ifndef EVENT_BUTTON_HTMLCOMPONENTBUILDER_H
#define EVENT_BUTTON_HTMLCOMPONENTBUILDER_H

#include <Arduino.h>

class ButtonState;

class HTMLComponentBuilder {
public:
    explicit HTMLComponentBuilder(const ButtonState &buttonState);

    String componentById(const String &ref) const;

private:
    const ButtonState &_buttonState;
};

#endif //EVENT_BUTTON_HTMLCOMPONENTBUILDER_H
