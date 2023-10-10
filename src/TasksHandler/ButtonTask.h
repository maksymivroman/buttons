//
// Created by rmaks on 06-Aug-23.
//

#ifndef EVENT_BUTTON_BUTTONTASK_H
#define EVENT_BUTTON_BUTTONTASK_H

#include <Arduino.h>

class ButtonTask {

public:
    ButtonTask()= default;
    explicit ButtonTask(bool startWith) {
        this->distinctChangeFlag = startWith;
    };

    template<typename FnTrue>
    void operator()(bool truthHandler, FnTrue execute, bool skipTask = false){
        if ((truthHandler != this->distinctChangeFlag) & !skipTask) {
            Serial.println("[ButtonTask]-> truth handler");
            this->distinctChangeFlag = truthHandler;
            if (truthHandler) execute();
        }
    };

    template<typename FnTrue, typename FnFalse>
    void operator()(bool handler, FnTrue executeOnTrue, FnFalse executeOnFalse, bool skipTask = false){
        if ((handler != this->distinctChangeFlag) & !skipTask) {
            Serial.println("[ButtonTask]-> distinct handler");
            this->distinctChangeFlag = handler;
            handler ? executeOnTrue() : executeOnFalse();
        }
    };

private:
    bool distinctChangeFlag{false};

};


#endif //EVENT_BUTTON_BUTTONTASK_H
