//
// Created by rmaks on 12-Sep-24.
//

#ifndef EVENT_BUTTON_BUTTONSTATE_H
#define EVENT_BUTTON_BUTTONSTATE_H

enum BUTTON_STATE {
    NOT_PRESSED,
    PRESSED
};

class ButtonState {

public:
    void setState(int state);
    BUTTON_STATE getState() const;
    bool isPressed() const;

    void setToggleState(BUTTON_STATE state);
    BUTTON_STATE toggleState();
    BUTTON_STATE getToggleMode() const;

private:
    int currentButtonState = 0;
    int toggleableState = 0;
};


#endif //EVENT_BUTTON_BUTTONSTATE_H
