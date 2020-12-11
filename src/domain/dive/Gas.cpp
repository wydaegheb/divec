#include "Gas.h"

#include <utility>


// Note to self: do not ever, ever put Serial.print in static constructors!!!!
// Gets called before Serial is initialised -> crashes the board and takes ages to deduce what happened
Gas::Gas(double o2, char const *name) {
    _o2 = o2;
    _n2 = 1.0 - _o2;
    _he = 0.0;
    _name = name;
    _active = true;

    resolveName();
}

Gas::Gas(double o2, double he, char const *name) {
    _o2 = o2;
    _he = he;
    _n2 = 1.0 - _o2 - _he;
    _name = name;
    _active = true;

    resolveName();
}

Gas::Gas(double o2, double he, char const *name, bool active) {
    _o2 = o2;
    _he = he;
    _n2 = 1.0 - _o2 - _he;
    _name = name;
    _active = active;

    resolveName();
}

bool Gas::isActive() const {
    return _active;
}

void Gas::setActive(bool active) {
    _active = active;
}

char const *Gas::getName() {
    return _name;
}

double Gas::getO2() const {
    return _o2;
}

void Gas::setO2(double o2) {
    _o2 = o2;
}

double Gas::getN2() const {
    return _n2;
}

void Gas::setN2(double n2) {
    _n2 = n2;
}

double Gas::getHe() const {
    return _he;
}

void Gas::setHe(double he) {
    _he = he;
}

uint16_t Gas::calcMODInMeters() const { // result is max depth in meters before O2 becomes toxic
    return DiveEquations::barToDepthInMeters(Settings::MAX_PPO2 / _o2);
}

uint16_t Gas::calcENDInMeters(double pressureInBars) const { // result is equivalent narcotic depth in meters at the given actual pressure (input given in bars)
    double narcIndex = _n2;
    if (Settings::O2_NARCOTIC) {
        narcIndex += _o2;
    }
    return DiveEquations::barToDepthInMeters(pressureInBars * narcIndex);
}

bool Gas::isUsable(double pressureInBars) const {
    return (pressureInBars * _o2 < Settings::MAX_PPO2) && (calcENDInMeters(pressureInBars) < Settings::MAX_END);
}

void Gas::resolveName() {
    uint8_t num = (uint8_t) (_o2 * 100);

    char o2Str[3];
    utoa(num, o2Str, 10);

    char heStr[3];
    utoa(num, heStr, 10);

    strcat(_mix, o2Str);
    strcat(_mix, "/");
    strcat(_mix, heStr);

    if (!_name) {
        _name = _mix;
    }
}

JsonObject Gas::serializeObject(JsonObject &doc) {
    doc["name"] = getName();
    doc["o2"] = getO2();
    doc["he"] = getHe();
    doc["active"] = isActive();
    return doc;
}

void Gas::deserializeObject(JsonObject &doc) {
    _name = doc["name"];
    _o2 = doc["o2"];
    _he = doc["he"];
    _active = doc["active"];
    resolveName();
}

size_t Gas::getJsonSize() {
    return JSON_OBJECT_SIZE(4); // 4 properties
}





