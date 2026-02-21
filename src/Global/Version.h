//
// Created by rmaks on 31-Dec-23.
//

#ifndef EVENT_BUTTON_VERSION_H
#define EVENT_BUTTON_VERSION_H

#include <Arduino.h>
#include <sstream>
#include <map>

#include "Global/Global.hpp"

enum BOARD_VERSION {
    UNDEFINED, REV_1, REV_2, REV_3
};

struct ADCRange {
    int min;
    int max;
};

typedef BOARD_VERSION PCBBoardVersion;
typedef std::map<BOARD_VERSION, ADCRange> PCBBoardVersionMap;

class Version {

public:
    explicit Version(unsigned int major, unsigned int minor, unsigned int patch, bool requireEEPROMFormat);

    bool operator!=(const Version &version) const;

    String str_version() const;
    String str_fullVersion() const;

    unsigned int uint_version() const;
    bool EEPROMStructureChanged() const;

    PCBBoardVersion getBoardVersion();
    String getBoardName();

private:
    unsigned int major;
    unsigned int minor;
    unsigned int patch;
    bool requireEEPROMFormat;

    PCBBoardVersionMap _boardVersionsByADC{
            {REV_3, {90, 120}},
            {UNDEFINED, {0, 50}}
    };

};


#endif //EVENT_BUTTON_VERSION_H
