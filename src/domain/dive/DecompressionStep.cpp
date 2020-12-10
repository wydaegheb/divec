#include "DecompressionStep.h"

DecompressionStep::DecompressionStep(Gas *diveGas, uint16_t durationInSeconds, uint16_t depthInMeters) {
    setDiveGas(diveGas);
    setDurationInSeconds(durationInSeconds);
    setStartDepthInMeters(depthInMeters);
    setEndDepthInMeters(depthInMeters);
}


DecompressionStep::DecompressionStep(Gas *diveGas, uint16_t durationInSeconds, uint16_t startDepthInMeters, uint16_t endDepthInMeters) {
    setDiveGas(diveGas);
    setDurationInSeconds(durationInSeconds);
    setStartDepthInMeters(startDepthInMeters);
    setEndDepthInMeters(endDepthInMeters);
}


DecompressionStep::~DecompressionStep() {
    _diveGas = nullptr; // This step just contains the pointer. the gasmanager is master of the pool of gasses. so do not delete _divegas here.
}

Gas *DecompressionStep::getDiveGas() {
    return _diveGas;
}

void DecompressionStep::setDiveGas(Gas *diveGas) {
    _diveGas = diveGas;
}

uint16_t DecompressionStep::getStartDepthInMeters() const {
    return _startDepthInMeters;
}

void DecompressionStep::setStartDepthInMeters(uint16_t startDepthInMeters) {
    _startDepthInMeters = startDepthInMeters;
}

uint16_t DecompressionStep::getEndDepthInMeters() const {
    return _endDepthInMeters;
}

void DecompressionStep::setEndDepthInMeters(uint16_t endDepthInMeters) {
    _endDepthInMeters = endDepthInMeters;
}

uint16_t DecompressionStep::getDurationInSeconds() const {
    return _durationInSeconds;
}

void DecompressionStep::setDurationInSeconds(uint16_t durationInSeconds) {
    _durationInSeconds = durationInSeconds;
}

bool DecompressionStep::isFlat() const {
    return (_startDepthInMeters == _endDepthInMeters);
}

uint32_t DecompressionStep::log(uint32_t time) {
    return log(&Serial, time);
}

uint32_t DecompressionStep::log(Print *print, uint32_t time) {
    print->print(getEndDepthInMeters());
    print->print(F("\t\t"));
    if (isFlat()) {
        char timeStr[9] = "";
        print->print(Formatter::formatTime(timeStr, getDurationInSeconds(), true));
        print->print(F("\t\t"));
    } else {
        print->print(F(" - "));
        print->print(F("\t\t\t"));
    }
    char timeStr[6] = "";
    print->print(Formatter::formatTimeInMinutes(timeStr, time + getDurationInSeconds(), Settings::MIN_STOP_TIME >= 60));
    print->print(F("\t\t"));
    print->println((getDiveGas()->getName()));

    return (time + getDurationInSeconds());

}










