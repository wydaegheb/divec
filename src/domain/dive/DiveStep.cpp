#include <RTClib.h>
#include "DiveStep.h"

DiveStep::DiveStep(DateTime endTime, Gas *gas, double pressureInBar, double temperatureInCelsius) {
    setEndTime(endTime);
    setGasName(gas);
    setPressureInBar(pressureInBar);
    setTemperatureInCelsius(temperatureInCelsius);
}

DateTime DiveStep::getEndTime() {
    return _endTime;
}

void DiveStep::setEndTime(DateTime endTime) {
    _endTime = endTime;
}

double DiveStep::getPressureInBar() {
    return _pressureInBar;
}

void DiveStep::setPressureInBar(double pressureInBar) {
    _pressureInBar = pressureInBar;
}

double DiveStep::getTemperatureInCelsius() {
    return _temperatureInCelsius;
}

void DiveStep::setTemperatureInCelsius(double temperatureInCelsius) {
    _temperatureInCelsius = temperatureInCelsius;
}

char const* DiveStep::getGasName() {
    return _gasName;
}

void DiveStep::setGasName(Gas *gas) {
    _gasName = gas->getName();
}

void DiveStep::log() {
    Serial.print(_endTime.timestamp(DateTime::TIMESTAMP_TIME));
    Serial.print(F("\t\t"));
    Serial.print(DiveEquations::barToDepthInMeters(_pressureInBar));
    Serial.print(F("\t\t"));
    Serial.print(_gasName);
    Serial.print(F("\t\t"));
    Serial.println(_temperatureInCelsius);
}


