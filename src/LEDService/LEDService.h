#ifndef EVENT_BUTTON_LEDSERVICE_H
#define EVENT_BUTTON_LEDSERVICE_H


#include "Global/Global.hpp"

class LEDService {
public:
    void pinConfig(int r, int g, int b);

    void lightOnRed(bool on);
    void lightOnGreen(bool on);
    void lightOnBlue(bool on);
    void lightOnPurple(bool on);
    void blinkWarn();
    void blinkPrimary();
    void blinkDone();
    void findMe();
    void eventsSendInProgress(bool on);
    void updateKeystoreProgress(bool on);
    void idle();

private:
    int pinRed;
    int pinGreen;
    int pinBlue;
    RGBCONFIG rgbCurrentState;

    void switchPin(RGBCONFIG pinsState = {0,0,0}) const;
    void switchPin(int pin, int state) const;
    void blink(int pin, int count);
    void saveCurrentRGBState();
    void restoreCurrentRGBState();

};


#endif
