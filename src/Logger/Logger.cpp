//
// Created by rmaks on 11-Oct-23.
//

#include "Logger.h"

void Logger::start() {
    Serial.begin(115200);
    Serial.println();
    Serial.println("[Init logger]");
    this->enabled = true;
}

void Logger::stop() {
    Serial.end();
    this->enabled = false;
}

bool Logger::loggerEnabled() {
    return this->enabled;
}

bool Logger::canLog() {
    return this->enabled;
}
