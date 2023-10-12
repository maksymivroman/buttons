//
// Created by rmaks on 11-Oct-23.
//

#ifndef EVENT_BUTTON_LOGGER_H
#define EVENT_BUTTON_LOGGER_H

#include <Arduino.h>

class Logger {

public:
    template<typename T, typename ... Args>
    void log(const T &messageItem, const Args &...arguments);

    template<typename T>
    void log(const T &message);

    template<typename T>
    void append(const T &message);

    void start();

    void stop();

    bool loggerEnabled();


private:
    bool enabled{false};

    bool canLog();

};


template<typename T, typename... Args>
void Logger::log(const T &messageItem, const Args &... arguments) {
    if (this->canLog()) {
        Serial.print(messageItem);
        log(arguments...);
    }
}

template<typename T>
void Logger::log(const T &message) {
    if (this->canLog()) {
        Serial.println(message);
    }
}

template<typename T>
void Logger::append(const T &message) {
    if (this->canLog()) {
        Serial.print(message);
    }
}

#endif //EVENT_BUTTON_LOGGER_H
