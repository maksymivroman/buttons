//
// Created by rmaks on 12-Sep-24.
//

#ifndef EVENT_BUTTON_BUTTONSTATE_H
#define EVENT_BUTTON_BUTTONSTATE_H

#include "Global/Global.hpp"
#include <map>

typedef std::map<BUTTON_STATE, EVENT_TRIGGER> EventTriggerMap;

class ButtonState {

public:
    void setState(int state);
    void toggleState();
    BUTTON_STATE getState() const;
    bool isPressed() const;

    void setOperationMode(OPERATION_MODE mode);
    OPERATION_MODE getOperationMode() const;

    bool isSetupOperationMode() const;
    bool isRunOperationMode() const;

    void setToggleState(BUTTON_STATE state);
    BUTTON_STATE toggleToggleState();
    BUTTON_STATE getToggleMode() const;
    EVENT_TRIGGER getEventTrigger(bool isToggleMode);

private:
    int currentButtonState = 0;
    int toggleableState = 0;
    OPERATION_MODE _operationMode {RUN};
    EventTriggerMap triggerByState = {
            {NOT_PRESSED, STATE_RELEASED},
            {PRESSED, STATE_PRESSED}
    };
};


#endif //EVENT_BUTTON_BUTTONSTATE_H
