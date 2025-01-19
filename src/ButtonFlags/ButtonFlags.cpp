//
// Created by rmaks on 09-Dec-24.
//

#include "ButtonFlags.h"

bool ButtonFlags::isValidFlag(String &flag) const {
    return std::find(this->flags.begin(), this->flags.end(),flag) != this->flags.end();
}

void ButtonFlags::updateFlags(EEPROM_FLAGS *eepromFlagsPtr, String &flag, bool value) {
    if (flag == "ledRDisabled") {
        eepromFlagsPtr->ledRDisabled = value;
    } else if (flag == "ledGDisabled") {
        eepromFlagsPtr->ledGDisabled = value;
    } else if (flag == "ledBDisabled") {
        eepromFlagsPtr->ledBDisabled = value;
    }
}
