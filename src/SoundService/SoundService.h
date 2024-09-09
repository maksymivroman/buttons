//
// Created by rmaks on 13.04.2023.
//

#ifndef INDEX_HTML_SOUNDSERVICE_H
#define INDEX_HTML_SOUNDSERVICE_H

#include <Arduino.h>

class SoundService {

public:
    explicit SoundService(int pin) { buzzerPin = pin; }

    void onRestart();
    void onFindMe();
    void onIntegrationMessage();
    void onRemoteTrigger();
    void onExternal();

    bool useSound = false;

private:
    int buzzerPin{};

};


#endif //INDEX_HTML_SOUNDSERVICE_H
