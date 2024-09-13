#include <Arduino.h>
#include "LEDService.h"

unsigned char RGB_IO::read(IO_PINS pin) const {
    return _rgbStateMap.at(pin);
}

void RGB_IO::write(IO_PINS pin, unsigned char value)  {
    auto ioPin = this->ioPinConfig.at(pin);
    analogWrite(ioPin, value);
    _rgbStateMap[static_cast<IO_PINS>(pin)] = value;
}

void RGB_IO::initIOPins(unsigned char r, unsigned char g, unsigned char b) {
    this->ioPinConfig = {{ R_PIN, r }, {G_PIN, g} , { B_PIN, b }};
    pinMode(r, OUTPUT);
    pinMode(g, OUTPUT);
    pinMode(b, OUTPUT);
}


void LEDService::pinConfig(int r, int g, int b) {
    this->initIOPins(r, g, b);
}

void LEDService::switchPin(IO_PINS pin, int state) {
    write(R_PIN, 0);
    write(G_PIN, 0);
    write(B_PIN, 0);
    write(pin, state);
}

void LEDService::switchPin(RGBCONFIG pinsState) {
    write(R_PIN, pinsState.r);
    write(G_PIN, pinsState.g);
    write(B_PIN, pinsState.b);
}

void LEDService::switchAnalogPin(RGBCONFIG pinsState) {
    write(R_PIN, pinsState.r);
    write(G_PIN, pinsState.g);
    write(B_PIN, pinsState.b);
}

void LEDService::blink(IO_PINS pin, int count) {
    for (int i = 0; i < count * 2; ++i) {
        unsigned char state = read(pin);
        switchPin(pin, ~state);
        delay(100);
    }
    switchPin(pin, 0);
}

void LEDService::lightOnRed(bool on) {
    on ? switchAnalogPin(RGB._red) : switchAnalogPin(RGB._off);
}

void LEDService::lightOnBlue(bool on) {
    on ? switchAnalogPin(RGB._blue) : switchAnalogPin(RGB._off);
}

void LEDService::lightOnGreen(bool on) {
    on ? switchAnalogPin(RGB._green) : switchAnalogPin(RGB._off);
}

void LEDService::lightOnPurple(bool on) {
    on ? switchAnalogPin(RGB._purple) : switchAnalogPin(RGB._off);
}

void LEDService::blinkWarn() {
    blink(R_PIN, 6);
}

void LEDService::blinkPrimary() {
    blink(B_PIN, 5);
}

void LEDService::blinkDone() {
    blink(G_PIN, 5);
}

void LEDService::saveCurrentRGBState() {
    rgbCurrentState = {read(R_PIN),read(G_PIN),read(B_PIN)};
}

void LEDService::findMe() {
    saveCurrentRGBState();
    blinkWarn();
    blinkPrimary();
    restoreCurrentRGBState();
}

void LEDService::restoreCurrentRGBState() {
    write(R_PIN, rgbCurrentState.r);
    write(G_PIN, rgbCurrentState.g);
    write(B_PIN, rgbCurrentState.b);
}

void LEDService::eventsSendInProgress(bool on) {
    if (on) {
        if (!externalInterfaceOn) saveCurrentRGBState();
        lightOnBlue(true);
    } else {
        restoreCurrentRGBState();
    }
}

void LEDService::updateKeystoreProgress(bool on) {
    if (on) {
        if (!externalInterfaceOn) saveCurrentRGBState();
        lightOnPurple(true);
    } else {
        restoreCurrentRGBState();
    }
}

void LEDService::onExternalInterfaceProgress(bool on) {
    if (on) {
        this->toggleExternalInterfaceProgress();
    } else {
        restoreCurrentRGBState();
    }
}

void LEDService::toggleExternalInterfaceProgress() {
    if (externalInterfaceOn) {
        restoreCurrentRGBState();
        externalInterfaceOn = false;
    } else {
        switchAnalogPin(externalIMode);
        externalInterfaceOn = true;
    }
}

void LEDService::onFormatFS() {
    lightOnRed(true);
}

void LEDService::onNoConnection() {
    lightOnRed(true);
    saveCurrentRGBState();
}

void LEDService::idle() {
    switchAnalogPin(RGB._green);
    saveCurrentRGBState();
}

void LEDService::idle(bool isToggleMode, bool isPressedState) {
    if (!isToggleMode) {
        this->idle();
        return;
    }
    isPressedState ? switchAnalogPin(RGB._orange) : switchAnalogPin(RGB._green);
    saveCurrentRGBState();
}

void LEDService::idle(bool isPressedState) {
    isPressedState ? switchAnalogPin(RGB._orange) : switchAnalogPin(RGB._green);
    saveCurrentRGBState();
}
