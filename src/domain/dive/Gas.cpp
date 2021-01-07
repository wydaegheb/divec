#include "Gas.h"

// Note to self: do not ever, ever put Serial.print in static constructors!!!!
// Gets called before Serial is initialised -> crashes the board and takes ages to deduce what happened
Gas::Gas(uint8_t o2, GasType gasType) {
    _o2 = o2;
    _n2 = 100 - _o2;
    _he = 0;
    _active = true;
    _gasType = gasType;
}

Gas::Gas(uint8_t o2, uint8_t he, GasType gasType) {
    _o2 = o2;
    _he = he;
    _n2 = 100 - _o2 - _he;
    _active = true;
    _gasType = gasType;
}

Gas::Gas(uint8_t o2, uint8_t he, bool active, GasType gasType) {
    _o2 = o2;
    _he = he;
    _n2 = 100 - _o2 - _he;
    _active = active;
    _gasType = gasType;
}

GasType Gas::getGasType() const {
    return _gasType;
}

void Gas::setGasType(GasType gasType) {
    _gasType = gasType;
}

bool Gas::isActive() const {
    return _active;
}

void Gas::setActive(bool active) {
    _active = active;
}

char const *Gas::getName() {
    char o2Str[3];
    utoa(_o2, o2Str, 10);

    char heStr[3];
    utoa(_he, heStr, 10);

    _name[0] = 0;
    strcat(_name, o2Str);
    strcat(_name, "/");
    strcat(_name, heStr);
}

double Gas::getO2frac() const {
    return _o2 / 100.0;
}

uint8_t Gas::getO2() const {
    return _o2;
}

void Gas::setO2(uint8_t o2) {
    _o2 = o2;
}

double Gas::getN2frac() const {
    return _n2 / 100.0;
}

uint8_t Gas::getN2() const {
    return _n2;
}

void Gas::setN2(uint8_t n2) {
    _n2 = n2;
}

double Gas::getHefrac() const {
    return _he / 100.0;
}

uint8_t Gas::getHe() const {
    return _he;
}

void Gas::setHe(uint8_t he) {
    _he = he;
}

uint16_t Gas::calcMODInMeters() const { // result is max depth in meters before O2 becomes toxic
    return DiveEquations::barToDepthInMeters(Settings::MAX_PPO2 / getO2frac());
}

uint16_t Gas::calcENDInMeters(double pressureInBars) const { // result is equivalent narcotic depth in meters at the given actual pressure (input given in bars)
    double narcIndex = getN2frac();
    if (Settings::O2_NARCOTIC) {
        narcIndex += getO2frac();
    }
    return DiveEquations::barToDepthInMeters(pressureInBars * narcIndex);
}

bool Gas::isUsable(double pressureInBars) const {
    return (pressureInBars * getO2frac() < Settings::MAX_PPO2) && (calcENDInMeters(pressureInBars) < Settings::MAX_END);
}

JsonObject Gas::serializeObject(JsonObject &doc) {
    doc["o2"] = _o2;
    doc["he"] = _he;
    doc["type"] = getGasType();
    doc["active"] = isActive();
    return doc;
}

void Gas::deserializeObject(JsonObject &doc) {
    _o2 = doc["o2"];
    _he = doc["he"];
    _gasType = doc["type"];
    _active = doc["active"];
}

size_t Gas::getJsonSize() {
    return JSON_OBJECT_SIZE(4); // 4 properties
}











