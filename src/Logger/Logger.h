//
// Created by rmaks on 11-Oct-23.
//

#ifndef EVENT_BUTTON_LOGGER_H
#define EVENT_BUTTON_LOGGER_H

#include <Arduino.h>
#include "ArduinoJson.h"

class Logger {

public:
    Logger(unsigned long bound, uint maxLogItems);

    template<typename T, typename ... Args>
    void log(const T &messageItem, const Args &...arguments);

    template<typename T>
    void log(const T &message);

    template<typename T, typename ... Args>
    void logSerial(const T &messageItem, const Args &...arguments);

    template<typename T>
    void logSerial(const T &message);

    void start();

    void stop();

    [[nodiscard]] std::vector<String> logs() const;


private:
    bool enabled{false};
    const unsigned long bound;
    const uint itemsCount;

    [[nodiscard]] bool canLog() const;

    void logMessage(const String &s, bool append);
    void logMessage(const char str[], bool append);
    void logMessage(const int &i, bool append);
    void logMessage(const JsonObject &j, bool append);

    void addMessageToLog(String &message);

    void normalizeLogItem(String &message);

    std::vector<String> logsData;

    String messageBuffer;
};

template<typename T, typename... Args>
void Logger::log(const T &messageItem, const Args &... arguments) {
    if (this->canLog()) {
        this->logMessage(messageItem, true);
        log(arguments...);
    }
}

template<typename T>
void Logger::log(const T &messageItem) {
    if (this->canLog()) {
        this->logMessage(messageItem, false);
    }
}

template<typename T, typename... Args>
void Logger::logSerial(const T &messageItem, const Args &... arguments) {
    if (this->canLog()) {
        Serial.print(messageItem);
        logSerial(arguments...);
    }
}

template<typename T>
void Logger::logSerial(const T &messageItem) {
    if (this->canLog()) {
        Serial.println(messageItem);
    }
}


#endif //EVENT_BUTTON_LOGGER_H
