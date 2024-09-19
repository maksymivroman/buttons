#ifndef EVENT_BUTTON_LEDSERVICE_H
#define EVENT_BUTTON_LEDSERVICE_H


#include "Global/Global.hpp"
#include <map>

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

enum IO_PINS {
    R_PIN, G_PIN, B_PIN
};

typedef std::map<IO_PINS, int> RGBStateMap;
typedef std::map<IO_PINS, int> RGBIOPinsMap;

class RGB_IO {

public:
    unsigned char read(IO_PINS pin) const;
    void write(IO_PINS pin, unsigned char value);

    void initIOPins(unsigned char r, unsigned char g, unsigned char b);

private:
    RGBIOPinsMap ioPinConfig;
    RGBStateMap _rgbStateMap{{R_PIN, 0}, {G_PIN, 0}, {B_PIN, 0}};
};



class LEDService : private RGB_IO {
public:
    void pinConfig(int r, int g, int b);
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
    void idle(bool isToggleMode, bool isPressedState);
    void idle( bool isPressedState);

private:
    const RGB_def RGB;
    RGBCONFIG externalIMode{RGB._ocean};

    RGBCONFIG rgbCurrentState;
    bool externalInterfaceOn{false};

    void switchPin(RGBCONFIG pinsState = {0,0,0});
    void switchPin(IO_PINS pin, int state);
    void switchAnalogPin(RGBCONFIG pinsState = {0,0,0});
    void blink(IO_PINS pin, int count);
    void saveCurrentRGBState();
    void restoreCurrentRGBState();
    void toggleExternalInterfaceProgress();

};


#endif
