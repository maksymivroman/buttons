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
    ButtonTask() = default;
    explicit ButtonTask(bool startWith) {
        this->distinctChangeFlag = startWith;
    };

    template<typename FnTrue>
    void operator()(bool handler, FnTrue execute, bool skipTask = false){
        if ((handler != this->distinctChangeFlag) && !skipTask) {
            logger.log("[ButtonTask] Execute task handler (one time)");
            this->distinctChangeFlag = handler;
            if (handler) execute();
        }
    };

    template<typename FnTrue, typename FnFalse>
    void operator()(bool handler, FnTrue executeOnTrue, FnFalse executeOnFalse, bool skipTask = false){
        if ((handler != this->distinctChangeFlag) && !skipTask) {
            logger.log("[ButtonTask] Execute task handler (conditional)");
            this->distinctChangeFlag = handler;
            handler ? executeOnTrue() : executeOnFalse();
        }
    };

    template<typename FnTrue, typename FnFalse, typename FnSkip>
    void operator()(bool handler, FnTrue executeOnTrue, FnFalse executeOnFalse, FnSkip skipTaskFn){
        const bool skip = skipTaskFn();
        if ((handler != this->distinctChangeFlag) && !skip) {
            logger.log("[ButtonTask] Execute task handler (conditional, skip task)");
            this->distinctChangeFlag = handler;
            handler ? executeOnTrue() : executeOnFalse();
        }
    };

private:
    bool distinctChangeFlag{false};

};

class ButtonIntervalTask {

public:
    template<typename Fn>
    void operator()(unsigned long interval, Fn execute, bool skipTask = false){
        unsigned long currentTime = millis();
        if ((currentTime >= this->executeTime) && !skipTask) {
            logger.logSerial("[ButtonIntervalTask] Execute task handler (interval): ", interval);
            this->executeTime = currentTime + interval;
            execute();
        }
    };

private:
    unsigned long executeTime = 0;
};


#endif //EVENT_BUTTON_BUTTONTASK_H
