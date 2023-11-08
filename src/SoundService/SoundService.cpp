//
// Created by rmaks on 13.04.2023.
//

#include "SoundService.h"
#include "Global/Global.hpp"

void SoundService::onFindMe() {
    logger.log("[SoundService] useSound onFindMe: ", useSound);
    if (useSound) {
        for (int i = 0; i < 10; i++) {
            tone(buzzerPin, 800 * i, 200);
            delay(100);
        }
        noTone(buzzerPin);
    }
}

void SoundService::onRestart() {
    logger.log("[SoundService] useSound onRestart: ", useSound);
    if (useSound) {
        for (int i = 0; i < 5; i++) {
            tone(buzzerPin, 500 * i, 50);
            delay(100);
        }
        noTone(buzzerPin);
    }
}

void SoundService::onIntegrationMessage() {
    logger.log("[SoundService] useSound onIntegrationMessage: ", useSound);
    if (useSound) {
        for (int i = 0; i < 3; i++) {
            tone(buzzerPin, 150 * i, 150);
            delay(100);
        }
        noTone(buzzerPin);
    }
}

void SoundService::onRemoteTrigger() {
    logger.log("[SoundService] useSound onRemoteTrigger: ", useSound);
    if (useSound) {
        for (int i = 0; i < 3; i++) {
            tone(buzzerPin, 350 * i, 100);
            delay(100);
        }
        noTone(buzzerPin);
    }
}
