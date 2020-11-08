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
    Serial.print(getEndDepthInMeters());
    Serial.print(F("\t\t"));
    if (isFlat()) {
        char timeStr[9] = "";
        Serial.print(Formatter::formatTime(timeStr, getDurationInSeconds(), true));
        Serial.print(F("\t\t"));
    } else {
        Serial.print(F(" - "));
        Serial.print(F("\t\t\t"));
    }
    char timeStr[6] = "";
    Serial.print(Formatter::formatTimeInMinutes(timeStr, time + getDurationInSeconds(), Settings::MIN_STOP_TIME >= 60));
    Serial.print(F("\t\t"));
    Serial.println((getDiveGas()->getName()));

    return (time + getDurationInSeconds());

}








