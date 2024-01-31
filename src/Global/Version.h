//
// Created by rmaks on 31-Dec-23.
//

#ifndef EVENT_BUTTON_VERSION_H
#define EVENT_BUTTON_VERSION_H

#include <Arduino.h>
#include <sstream>

#include "Global/Global.hpp"


class Version {

public:
    explicit Version(unsigned int major, unsigned int minor, unsigned int patch, bool requireEEPROMFormat);

    bool operator!=(const Version &version) const;

    String str_version() const;
    String str_fullVersion() const;

    unsigned int uint_version() const;
    bool EEPROMStructureChanged() const;

private:
    unsigned int major;
    unsigned int minor;
    unsigned int patch;
    bool requireEEPROMFormat;
};


#endif //EVENT_BUTTON_VERSION_H
