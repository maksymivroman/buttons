//
// Created by rmaks on 27.02.2023.
//

#include "HTMLComponentBuilder.h"
#include "HTMLPage/index-html-page.hpp"
#include "ButtonState/ButtonState.h"

HTMLComponentBuilder::HTMLComponentBuilder(const ButtonState &buttonState) : _buttonState(buttonState) {}

String HTMLComponentBuilder::componentById(const String &ref) const {
    if (ref == "CLIENT_MODE_OPTIONS") {
        if (!_buttonState.isRunOperationMode()) {
            return FPSTR(Components::CLIENT_MODE_OPTIONS);
        }
        return Components::NONE;
    }
    if (ref == "DEVICE_LOGO") {
        return FPSTR(Components::DEVICE_LOGO);
    }
    return ref;
}
