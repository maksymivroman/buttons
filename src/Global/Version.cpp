//
// Created by rmaks on 31-Dec-23.
//

#include "Version.h"

Version::Version(unsigned int major, unsigned int minor, unsigned int patch, bool requireEEPROMFormat) :
        major(major), minor(minor), patch(patch), requireEEPROMFormat(requireEEPROMFormat) {
}

bool Version::operator!=(const Version &version) const {
    return this->major != version.major ||
           this->minor != version.minor ||
           this->patch != version.patch;
}

String Version::str_version() const {
    std::ostringstream vs;
    vs << this->major << "." << this->minor << "." << this->patch;
    return vs.str().c_str();
}

String Version::str_fullVersion() const {
    String vs = this->str_version();
    vs.concat(" - build date: ");
    vs.concat(__DATE__);
    return vs;
}

unsigned int Version::uint_version() const {
    return this->major * 10000 + this->minor * 100 + this->patch;
}

bool Version::EEPROMStructureChanged() const {
    return this->requireEEPROMFormat;
}

PCBBoardVersion Version::getBoardVersion() {
    const auto adcValue = analogRead(A0);
    logger.logSerial("[Version] Get PCB version by ADC. ADC value: ", adcValue);
    for (const auto& [version, adcRange] : this->_boardVersionsByADC ) {
        if (adcValue >= adcRange.min && adcValue <= adcRange.max) {
            logger.logSerial("[Version] Version by ADC: ", version);
            return version;
        }
    }
    logger.logSerial("[Version] Version by ADC: UNSPECIFIED");
    return UNDEFINED;
}

String Version::getBoardName() {
    auto version = this->getBoardVersion();
    if (!version) {
        return "-";
    }
    return "rev" + String(version);
}
