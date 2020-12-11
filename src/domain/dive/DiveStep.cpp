#include <RTClib.h>
#include "DiveStep.h"


DiveStep::DiveStep(uint32_t endTime, Gas *gas, double pressureInBar, double temperatureInCelsius) {
    setEndTime(endTime);
    setGasName(gas);
    setPressureInBar(pressureInBar);
    setTemperatureInCelsius(temperatureInCelsius);
}


uint32_t DiveStep::getEndTime() {
    return _endTime;
}

void DiveStep::setEndTime(uint32_t endTime) {
    _endTime = endTime;
}

double DiveStep::getPressureInBar() const {
    return _pressureInBar;
}

void DiveStep::setPressureInBar(double pressureInBar) {
    _pressureInBar = pressureInBar;
}

double DiveStep::getTemperatureInCelsius() const {
    return _temperatureInCelsius;
}

void DiveStep::setTemperatureInCelsius(double temperatureInCelsius) {
    _temperatureInCelsius = temperatureInCelsius;
}

char const *DiveStep::getGasName() {
    return _gasName;
}

void DiveStep::setGasName(Gas *gas) {
    _gasName = gas->getName();
}

JsonObject DiveStep::serializeObject(JsonObject &doc) {
    doc["end_time"] = _endTime;
    doc["pres"] = _pressureInBar;
    doc["temp"] = _temperatureInCelsius;
    doc["gas"] = _gasName;
    return doc;
}

void DiveStep::deserializeObject(JsonObject &doc) {
    _endTime = doc["end_time"];
    _pressureInBar = doc["pres"];
    _temperatureInCelsius = doc["temp"];
    _gasName = doc["gas"];
}

size_t DiveStep::getJsonSize() {
    return JSON_OBJECT_SIZE(4); // 4 properties
}

void DiveStep::log() {
    log(&Serial);
}

void DiveStep::log(Print *print) {
    print->print(DateTime(_endTime).timestamp(DateTime::TIMESTAMP_TIME));
    print->print(F("\t\t"));
    print->print(DiveEquations::barToDepthInMeters(_pressureInBar));
    print->print(F("\t\t"));
    print->print(_gasName);
    print->print(F("\t\t"));
    print->println(_temperatureInCelsius);
}





