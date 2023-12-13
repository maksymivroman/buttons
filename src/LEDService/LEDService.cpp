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

void LEDService::blink(int pin, int count) {
    for (int i = 0; i < count * 2; ++i) {
        int state = digitalRead(pin);
        switchPin(pin, !state);
        delay(100);
    }
    switchPin(pin, 0);
}

void LEDService::lightOnRed(bool on) {
    const int state = on ? 1 : 0;
    switchPin(pinRed, state);
}

void LEDService::lightOnBlue(bool on) {
    const int state = on ? 1 : 0;
    switchPin(pinBlue, state);
}

void LEDService::lightOnGreen(bool on) {
    const int state = on ? 1 : 0;
    switchPin(pinGreen, state);
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

void LEDService::findMe() {
    saveCurrentRGBState();
    blinkWarn();
    blinkPrimary();
    restoreCurrentRGBState();
}

void LEDService::saveCurrentRGBState() {
    rgbCurrentState = {digitalRead(pinRed),digitalRead(pinGreen),digitalRead(pinBlue)};
}

void LEDService::restoreCurrentRGBState() {
    digitalWrite(pinRed, rgbCurrentState.r);
    digitalWrite(pinGreen, rgbCurrentState.g);
    digitalWrite(pinBlue, rgbCurrentState.b);
}

void LEDService::eventsSendInProgress(bool on) {
    if (on) {
        saveCurrentRGBState();
        lightOnBlue(true);
    } else {
        restoreCurrentRGBState();
    }
}

void LEDService::idle() {
//TODO standby RGB gradient
}
