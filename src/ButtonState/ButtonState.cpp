//
// Created by rmaks on 12-Sep-24.
//

#include "ButtonState.h"

void ButtonState::setState(int state) {
    this->currentButtonState = state;
}

BUTTON_STATE ButtonState::getState() const {
    return static_cast<BUTTON_STATE>(this->currentButtonState);
}

bool ButtonState::isPressed() const {
    return this->currentButtonState == 1;
}

BUTTON_STATE ButtonState::getToggleMode() const {
    return static_cast<BUTTON_STATE>(this->toggleableState);
}

BUTTON_STATE ButtonState::toggleState() {
    this->toggleableState = this->toggleableState ? 0 : 1;
    return static_cast<BUTTON_STATE>(this->toggleableState);
}

void ButtonState::setToggleState(BUTTON_STATE state) {
    this->toggleableState = state;
}
