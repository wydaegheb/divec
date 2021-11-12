#include "DecompressionStep.h"

DecompressionStep::DecompressionStep(Gas *diveGas, uint16_t durationInSeconds, uint16_t depthInMeters) {
    setDiveGas(diveGas);
    setDurationInSeconds(durationInSeconds);
    setStartDepthInMeters(depthInMeters);
    setEndDepthInMeters(depthInMeters);
}


DecompressionStep::DecompressionStep(Gas *diveGas, uint16_t durationInSeconds, uint16_t startDepthInMeters,
                                     uint16_t endDepthInMeters) {
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
    if (isFlat()) {
        print->print(F("\t\t\t\t\t"));
    }

    //depth
    print->print(getEndDepthInMeters());
    print->print(F("\t"));


    // ascend duration/stop
    if (isFlat()) {
        print->print(lround((getDurationInSeconds() + 30.0) / 60.0));
    } else {
        print->print(getDurationInSeconds() / 60.0);
    }

    print->print(F("\t"));

    // runtime
    uint16_t runtime = time + getDurationInSeconds();
    print->print(runtime / 60.0);
    print->print(F("\t"));

    // gas
    print->println((getDiveGas()->getName()));

    return runtime;
}










