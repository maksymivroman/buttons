#include <Arduino.h>
#include "LEDService.h"

unsigned char RGB_IO::read(IO_PINS pin) const {
    return _rgbStateMap.at(pin);
}

void RGB_IO::write(IO_PINS pin, unsigned char value, bool saveState)  {
    auto ioPin = this->ioPinConfig.at(pin);
    analogWrite(ioPin, value);
    if (saveState) _rgbStateMap[static_cast<IO_PINS>(pin)] = value;
}

void RGB_IO::initIOPins(unsigned char r, unsigned char g, unsigned char b) {
    this->ioPinConfig = {{ R_PIN, r }, {G_PIN, g} , { B_PIN, b }};
    pinMode(r, OUTPUT);
    pinMode(g, OUTPUT);
    pinMode(b, OUTPUT);
}

RGBCONFIG RGB_IO::readAll() const {
    return RGBCONFIG {
            this->read(R_PIN),
            this->read(G_PIN),
            this->read(B_PIN),
    };
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

void LEDService::switchPin(RGBCONFIG pinsState, bool saveState) {
    write(R_PIN, pinsState.r, saveState);
    write(G_PIN, pinsState.g, saveState);
    write(B_PIN, pinsState.b, saveState);
}

void LEDService::setLedAction(ACTIONS action, bool blink, bool saveState) {
    auto actionRGBConfig = this->_ledMap.at(action);
    bool isEnabled = this->isEnabled(actionRGBConfig);
    if (isEnabled) {
        blink ? blinkRGB(actionRGBConfig, 5) : switchPin(actionRGBConfig, saveState);
    }
}

void LEDService::blinkRGB(RGBCONFIG config, int count) {
    auto state = readAll();
    for (int i = 0; i < count * 2; ++i) {
        i % 2 ? switchPin(config) : switchPin({0,0,0});
        delay(100);
    }
    switchPin(state);
}

bool LEDService::isEnabled(RGBCONFIG config) const {
    auto [r, g, b] = config;
    return r + g + b > 0;
}

void LEDService::resetLedAction() {
    auto state = readAll();
    switchPin(state);
}

void LEDService::applyLedMap(LED_MAP ledMap) {
    logger.log("[LED service] apply new led map");
    this->_ledMap.at(ACTIONS::IDLE_DEFAULT) = ledMap.IDLE_DEFAULT;
    this->_ledMap.at(ACTIONS::IDLE_PRESSED) = ledMap.IDLE_PRESSED;
    this->_ledMap.at(ACTIONS::LOADING) = ledMap.LOADING;
    this->_ledMap.at(ACTIONS::WARN) = ledMap.WARN;
    this->_ledMap.at(ACTIONS::DONE) = ledMap.DONE;
    this->_ledMap.at(ACTIONS::KEYSTORE_UPDATE) = ledMap.KEYSTORE_UPDATE;
    this->_ledMap.at(ACTIONS::SEND_EVENTS) = ledMap.SEND_EVENTS;
    this->_ledMap.at(ACTIONS::EXTERNAL_INTERFACE) = ledMap.EXTERNAL_INTERFACE;
}
