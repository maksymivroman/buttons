//
// Created by rmaks on 01-Apr-24.
//

#include "ExternalInterfaceService.h"

void ExternalInterfaceService::handleTriggers(AsyncWebServerRequest *request,
                                             ExternalInterfaceService::OptionsToHandle optionsToSwitch) {
    unsigned int paramsCount = request->params();

    for (unsigned int i = 0; i < paramsCount; i++) {
        String paramName = request->getParam(i)->name();
        String paramValue = request->getParam(i)->value();

        for (const auto& [key, propRef]: optionsToSwitch) {
            if(this->optionsDef[key] == paramName) {
                PropertyTrigger switchTriggers = this->options[key];
                propRef = switchTriggers[paramValue];
            }
        }
    }
}
