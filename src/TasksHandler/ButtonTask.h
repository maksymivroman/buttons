//
// Created by rmaks on 06-Aug-23.
//

#ifndef EVENT_BUTTON_BUTTONTASK_H
#define EVENT_BUTTON_BUTTONTASK_H

#include <Arduino.h>
#include "Logger/Logger.h"

extern Logger logger;

class ButtonTask {

public:
    ButtonTask()= default;
    explicit ButtonTask(bool startWith) {
        this->distinctChangeFlag = startWith;
    };

    template<typename FnTrue>
    void operator()(bool handler, FnTrue execute, bool skipTask = false){
        if ((handler != this->distinctChangeFlag) & !skipTask) {
            logger.log("[ButtonTask] Execute task handler (one time)");
            this->distinctChangeFlag = handler;
            if (handler) execute();
        }
    };

    template<typename FnTrue, typename FnFalse>
    void operator()(bool handler, FnTrue executeOnTrue, FnFalse executeOnFalse, bool skipTask = false){
        if ((handler != this->distinctChangeFlag) & !skipTask) {
            logger.log("[ButtonTask] Execute task handler (conditional)");
            this->distinctChangeFlag = handler;
            handler ? executeOnTrue() : executeOnFalse();
        }
    };

private:
    bool distinctChangeFlag{false};

};


#endif //EVENT_BUTTON_BUTTONTASK_H
