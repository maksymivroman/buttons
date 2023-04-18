//
// Created by rmaks on 13.04.2023.
//

#include "SoundService.h"

void SoundService::onFindMe() {
    Serial.print("[SoundService] useSound onFindMe: "); Serial.println(useSound);
    if (useSound) {
        for (int i = 0; i < 10; i++) {
            tone(buzzerPin, 800 * i, 200);
            delay(100);
        }
        noTone(buzzerPin);
    }
}

void SoundService::onRestart() {
    Serial.print("[SoundService] useSound onRestart: "); Serial.println(useSound);
    if (useSound) {
        for (int i = 0; i < 5; i++) {
            tone(buzzerPin, 500 * i, 50);
            delay(100);
        }
        noTone(buzzerPin);
    }
}

void SoundService::onIntegrationMessage() {
    Serial.print("[SoundService] useSound onIntegrationMessage: "); Serial.println(useSound);
    if (useSound) {
        for (int i = 0; i < 3; i++) {
            tone(buzzerPin, 150 * i, 150);
            delay(100);
        }
        noTone(buzzerPin);
    }
}
