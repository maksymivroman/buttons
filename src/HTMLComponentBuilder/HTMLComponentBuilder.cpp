//
// Created by rmaks on 27.02.2023.
//

#include "HTMLComponentBuilder.h"
#include "HTMLPage/html-page.hpp"

void HTMLComponentBuilder::setClientMode(bool isClient) {
    this->isClientMode = isClient;
}

String HTMLComponentBuilder::componentById(const String &ref) {
    if (ref == "CLIENTMODEOPTIONS") {
        if (!isClientMode) {
            return FPSTR(Component::CLIENT_MODE_OPTIONS);
        }
        return Component::EMPTY;
    }
    return ref;
}
