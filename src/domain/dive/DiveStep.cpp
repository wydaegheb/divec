#include <RTClib.h>
#include "DiveStep.h"

DiveStep::DiveStep(const DateTime &endTime, Gas *gas, double pressureInBar, double temperatureInCelsius) {
    setEndTime(endTime);
    setGasName(gas);
    setPressureInBar(pressureInBar);
    setTemperatureInCelsius(temperatureInCelsius);
}



DateTime DiveStep::getEndTime() {
    return _endTime;
}

void DiveStep::setEndTime(const DateTime &endTime) {
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

size_t DiveStep::serialize(File *file) {
    DynamicJsonDocument doc(getFileSize());

    doc["endTime"] = _endTime.secondstime();
    doc["lastTimeStamp"] = _pressureInBar;
    doc["temperatureInCelsius"] = _temperatureInCelsius;
    doc["maxDepthInMeter"] = _gasName;

    return serializeJsonPretty(doc, *file);
}

DeserializationError DiveStep::deserialize(File *file) {
    DynamicJsonDocument doc(getFileSize());

    DeserializationError error = deserializeJson(doc, *file);
    if (error) { // stop deserializing if json parse failed
        return error;
    }

    _endTime = DateTime((uint32_t) doc["endTime"]);
    _pressureInBar = doc["lastTimeStamp"];
    _temperatureInCelsius = doc["temperatureInCelsius"];
    _gasName = doc["maxDepthInMeter"];

    return error;
}

size_t DiveStep::getFileSize() {
    return JSON_OBJECT_SIZE(4); // 4 properties
}

void DiveStep::log() {
    log(&Serial);
}

void DiveStep::log(Print *print) {
    print->print(_endTime.timestamp(DateTime::TIMESTAMP_TIME));
    print->print(F("\t\t"));
    print->print(DiveEquations::barToDepthInMeters(_pressureInBar));
    print->print(F("\t\t"));
    print->print(_gasName);
    print->print(F("\t\t"));
    print->println(_temperatureInCelsius);
}




