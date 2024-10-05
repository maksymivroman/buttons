#ifndef EVENT_BUTTON_LEDSERVICE_H
#define EVENT_BUTTON_LEDSERVICE_H


#include "Global/Global.hpp"
#include <map>

extern Logger logger;

enum IO_PINS {
    R_PIN, G_PIN, B_PIN
};

typedef std::map<IO_PINS, int> RGBStateMap;
typedef std::map<IO_PINS, int> RGBIOPinsMap;

class RGB_IO {

public:
    unsigned char read(IO_PINS pin) const;
    RGBCONFIG readAll() const;
    void write(IO_PINS pin, unsigned char value, bool saveState = true);

    void initIOPins(unsigned char r, unsigned char g, unsigned char b);

private:
    RGBIOPinsMap ioPinConfig;
    RGBStateMap _rgbStateMap{{R_PIN, 0}, {G_PIN, 0}, {B_PIN, 0}};
};



class LEDService : private RGB_IO {
public:
    void pinConfig(int r, int g, int b);
    void applyLedMap(LED_MAP ledMap);

    void setLedAction(ACTIONS action, bool blink = false, bool saveState = true);
    void resetLedAction();

private:
    LEDConfig _ledMap{
            {ACTIONS::IDLE_DEFAULT,       {0,   255, 0}},
            {ACTIONS::IDLE_PRESSED,       {220, 90,  0}},
            {ACTIONS::LOADING,            {0,   0,   255}},
            {ACTIONS::WARN,               {255, 0,   0}},
            {ACTIONS::DONE,               {0,   255, 0}},
            {ACTIONS::KEYSTORE_UPDATE,    {255, 0,   255}},
            {ACTIONS::SEND_EVENTS,        {0,   0,   255}},
            {ACTIONS::EXTERNAL_INTERFACE, {0,   235, 235}},
    };

    void switchPin(RGBCONFIG pinsState = {0,0,0}, bool saveState = true);
    void switchPin(IO_PINS pin, int state);

    void blinkRGB(RGBCONFIG config, int count);
    bool isEnabled(RGBCONFIG config) const;
};


#endif
