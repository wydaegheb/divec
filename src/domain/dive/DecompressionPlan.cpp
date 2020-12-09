#include "DecompressionPlan.h"

DecompressionPlan::~DecompressionPlan() {
    for (DecompressionStep *stop:_stops) {
        delete stop;
    }
    _stops.clear();
}

void DecompressionPlan::addStop(Gas *gas, uint32_t timeInSeconds, uint16_t depthInMeter) {
/*    Serial.print(F(" - Adding deco stop - duration:"));
    Serial.print(timeInSeconds);
    Serial.print(F(" sec - depth: "));
    Serial.print(depthInMeter);
    Serial.print(F(" m - gas:"));
    Serial.println(gas->getName());*/

    DecompressionStep *last = _stops.back();
    if (last->isFlat() && (last->getEndDepthInMeters() == depthInMeter)) { // collapse flat segments on the same depth
        last->setDurationInSeconds(last->getDurationInSeconds() + timeInSeconds);
/*        Serial.print(F(" collapse. new time is:"));
        Serial.println(last->getDurationInSeconds() + timeInSeconds);*/

    } else {
        _stops.emplace_back(new DecompressionStep(gas, timeInSeconds, depthInMeter, depthInMeter));
    }
}

void DecompressionPlan::addDecoDepthChange(Gas *gas, uint32_t timeInSeconds, uint16_t startDepthInMeter, uint16_t endDepthInMeter) {
/*    Serial.print(F(" - Adding deco ascent - duration:"));
    Serial.print(timeInSeconds);
    Serial.print(F(" sec - from depth: "));
    Serial.print(startDepthInMeter);
    Serial.print(F(" m to depth: "));
    Serial.println(endDepthInMeter);*/
    _stops.emplace_back(new DecompressionStep(gas, timeInSeconds, startDepthInMeter, endDepthInMeter));
}


std::list<DecompressionStep *> DecompressionPlan::getStops() {
    return _stops;
}

void DecompressionPlan::log() {
    log(nullptr);
}

void DecompressionPlan::log(Dive *dive) {
    log(&Serial, dive);
}

void DecompressionPlan::log(Print *print, Dive *dive) {
    uint32_t planTime = 0;
    print->println(F("\n=============================================="));
    print->print(F("Decompression Plan"));
    print->print(" (GF:");
    print->print(round(Settings::GF_LOW * 100));
    print->print(F("/"));
    print->print(round(Settings::GF_HIGH * 100));
    print->println(F(")"));
    print->println(F("=============================================="));
    print->println(F("Depth\tStop\t\tRun\t\tMix"));

    if (dive != nullptr) {
        uint32_t startTimeDiveInSeconds = dive->getStartTime().secondstime();
        uint32_t previousStepTimeInSeconds = startTimeDiveInSeconds;
        int16_t previousDepthInMeters = 0;
        for (DiveStep *step:dive->getSteps()) {
            double stepEndDepthInMeters = DiveEquations::barToDepthInMeters(step->getPressureInBar());
            print->print(stepEndDepthInMeters);
            print->print(F("\t\t"));
            if (previousDepthInMeters == stepEndDepthInMeters) {
                char timeStr[9] = "";
                print->print(Formatter::formatTime(timeStr, step->getEndTime().secondstime() - previousStepTimeInSeconds, true));
                print->print(F("\t\t"));
            } else {
                print->print(F(" - "));
                print->print(F("\t\t\t"));
            }
            char timeStr[6] = "";
            print->print(Formatter::formatTimeInMinutes(timeStr, step->getEndTime().secondstime() - startTimeDiveInSeconds, Settings::MIN_STOP_TIME >= 60));
            print->print(F("\t\t"));
            print->println(step->getGasName());
            previousStepTimeInSeconds = step->getEndTime().secondstime();
            previousDepthInMeters = stepEndDepthInMeters;
        }
        print->println();
        planTime = dive->getDiveTimeInSeconds();
    }

    for (DecompressionStep *stop:_stops) {
        planTime = stop->log(print, planTime);
    }
    print->println(F("=============================================="));
}


uint32_t DecompressionPlan::getTtsInSeconds() {
    double time = 0.0;
    for (DecompressionStep *stop:_stops) {
        time += stop->getDurationInSeconds();
    }
    return time;
}




