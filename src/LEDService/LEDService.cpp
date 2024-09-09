#include <Arduino.h>
#include "LEDService.h"

void LEDService::pinConfig(int r, int g, int b) {
    pinGreen = g;
    pinRed = r;
    pinBlue = b;
    pinMode(pinRed, OUTPUT);
    pinMode(pinGreen, OUTPUT);
    pinMode(pinBlue, OUTPUT);
}

void LEDService::switchPin(int pin, int state) const {
    digitalWrite(pinGreen, 0);
    digitalWrite(pinRed, 0);
    digitalWrite(pinBlue, 0);
    digitalWrite(pin, state);
}

void LEDService::switchPin(RGBCONFIG pinsState) const {
    digitalWrite(pinRed, pinsState.r);
    digitalWrite(pinGreen, pinsState.g);
    digitalWrite(pinBlue, pinsState.b);
}

void LEDService::switchAnalogPin(RGBCONFIG pinsState) const {
    analogWrite(pinRed, pinsState.r);
    analogWrite(pinGreen, pinsState.g);
    analogWrite(pinBlue, pinsState.b);
}

void LEDService::blink(int pin, int count) {
    for (int i = 0; i < count * 2; ++i) {
        int state = digitalRead(pin);
        switchPin(pin, !state);
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
    blink(pinRed, 6);
}

void LEDService::blinkPrimary() {
    blink(pinBlue, 5);
}

void LEDService::blinkDone() {
    blink(pinGreen, 5);
}

void LEDService::saveCurrentRGBState() {
    rgbCurrentState = {digitalRead(pinRed),digitalRead(pinGreen),digitalRead(pinBlue)};
}

void LEDService::findMe() {
    saveCurrentRGBState();
    blinkWarn();
    blinkPrimary();
    restoreCurrentRGBState();
}

void LEDService::restoreCurrentRGBState() {
    digitalWrite(pinRed, rgbCurrentState.r);
    digitalWrite(pinGreen, rgbCurrentState.g);
    digitalWrite(pinBlue, rgbCurrentState.b);
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

void LEDService::idle() {
    switchAnalogPin(RGB._green);
    saveCurrentRGBState();
}

void LEDService::onNoConnection() {
    lightOnRed(true);
    saveCurrentRGBState();
}

void LEDService::setCustomRGB(RGBCONFIG RGBConfig) {
    this->externalIMode = RGBConfig;
}
