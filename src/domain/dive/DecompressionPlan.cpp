#include "DecompressionPlan.h"

// Singleton to avoid having many of these floating around when calculating many of these.
// No need to have multiple instances of these in a dive computer
DecompressionPlan &DecompressionPlan::getInstance() {
    static DecompressionPlan instance;
    return instance;
}

DecompressionPlan::DecompressionPlan() {
    init();
}

void DecompressionPlan::init() {
    for (DecompressionStep *stop:_stops) {
        delete stop;
        stop = nullptr;
    }
    _nrOfStops = 0;
}

void DecompressionPlan::addStop(Gas *gas, uint32_t timeInSeconds, double depthInMeter) {
    DecompressionStep *last = nullptr;
    if (_nrOfStops > 0) {
        last = _stops[_nrOfStops - 1];
    }
    if (last && last->isFlat() &&
        (last->getEndDepthInMeters() == depthInMeter)) { // collapse flat segments on the same depth
        last->setDurationInSeconds(last->getDurationInSeconds() + timeInSeconds);
    } else {
        _stops[_nrOfStops] = new DecompressionStep(gas, timeInSeconds, depthInMeter, depthInMeter);
        _nrOfStops++;
    }
}

void DecompressionPlan::addDecoDepthChange(Gas *gas, uint32_t timeInSeconds, double startDepthInMeter,
                                           double endDepthInMeter) {
    _stops[_nrOfStops] = new DecompressionStep(gas, timeInSeconds, startDepthInMeter, endDepthInMeter);
    _nrOfStops++;
}

uint32_t DecompressionPlan::getTtsInSeconds() {
    double time = 0.0;
    for (uint8_t i = 0; i < _nrOfStops; i++) {
        time += _stops[i]->getDurationInSeconds();
    }
    return time;
}

DecompressionStep *DecompressionPlan::getFirstStop() {
    // return first "real" stop as the plan also contains the ascents between stops
    for (uint8_t i = 0; i < _nrOfStops; i++) {
        if (_stops[i]->isFlat()) {
            return _stops[i];
        }
    }
    return nullptr;
}

void DecompressionPlan::log(uint32_t planTime) {
    log(&Serial, planTime);
}

void DecompressionPlan::log(Print *print, uint32_t planTime) {

    print->println(F("\n=============================================="));
    print->print(F("Decompression Plan"));
    print->print(" (GF:");
    print->print(lround(Settings::GF_LOW * 100));
    print->print(F("/"));
    print->print(lround(Settings::GF_HIGH * 100));
    print->println(F(")"));
    print->println(F("=============================================="));
    print->println(F("Depth\tStop\tRun\tMix"));

    for (uint8_t i = 0; i < _nrOfStops; i++) {
        planTime = _stops[i]->log(planTime);
    }
    print->println(F("=============================================="));
}













