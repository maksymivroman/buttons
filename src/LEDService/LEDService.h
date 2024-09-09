#ifndef EVENT_BUTTON_LEDSERVICE_H
#define EVENT_BUTTON_LEDSERVICE_H


#include "Global/Global.hpp"
extern Logger logger;

struct RGB_def {
    const RGBCONFIG _off{0, 0, 0};
    const RGBCONFIG _red{255, 0, 0};
    const RGBCONFIG _green{0, 255, 0};
    const RGBCONFIG _blue{0, 0, 255};
    const RGBCONFIG _orange{220, 90, 0};
    const RGBCONFIG _purple{255, 0, 255};
    const RGBCONFIG _ocean{0, 235, 255};
};

class LEDService {
public:
    void pinConfig(int r, int g, int b);
    void setCustomRGB(RGBCONFIG externalIMode);

    void blinkWarn();
    void blinkPrimary();
    void blinkDone();

    void lightOnRed(bool on);
    void lightOnGreen(bool on);
    void lightOnBlue(bool on);
    void lightOnPurple(bool on);

    void findMe();
    void eventsSendInProgress(bool on);
    void updateKeystoreProgress(bool on);
    void onExternalInterfaceProgress(bool on);
    void onFormatFS();
    void onNoConnection();
    void idle();

private:
    const RGB_def RGB;
    RGBCONFIG externalIMode{RGB._ocean};

    int pinRed;
    int pinGreen;
    int pinBlue;
    RGBCONFIG rgbCurrentState;
    bool externalInterfaceOn{false};

    void switchPin(RGBCONFIG pinsState = {0,0,0}) const;
    void switchAnalogPin(RGBCONFIG pinsState = {0,0,0}) const;
    void switchPin(int pin, int state) const;
    void blink(int pin, int count);
    void saveCurrentRGBState();
    void restoreCurrentRGBState();
    void toggleExternalInterfaceProgress();

};


#endif
