//
// Created by rmaks on 09-Dec-24.
//

#ifndef EVENT_BUTTON_BUTTONFLAGS_H
#define EVENT_BUTTON_BUTTONFLAGS_H

#include <Arduino.h>
#include "Global/Global.hpp"
#include <vector>

class ButtonFlags {

public:
    bool isValidFlag(String &flag) const;
    void updateFlags(EEPROM_FLAGS *eepromFlagsPtr, String &flag, bool value);

private:

    typedef std::vector<String> Flags;
    Flags flags{"ledRDisabled","ledGDisabled","ledBDisabled"};

};


#endif //EVENT_BUTTON_BUTTONFLAGS_H
