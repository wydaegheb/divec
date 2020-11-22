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

char const *DiveStep::getGasName() {
    return _gasName;
}

void DiveStep::setGasName(Gas *gas) {
    _gasName = gas->getName();
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


