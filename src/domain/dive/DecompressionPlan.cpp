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
    Serial.println(F("\n=============================================="));
    Serial.print(F("Decompression Plan"));
    Serial.print(" (GF:");
    Serial.print(round(Settings::GF_LOW * 100));
    Serial.print(F("/"));
    Serial.print(round(Settings::GF_HIGH * 100));
    Serial.println(F(")"));
    Serial.println(F("=============================================="));
    Serial.println(F("Depth\tStop\t\tRun\t\tMix"));

    if (dive != nullptr) {
        uint32_t startTimeDiveInSeconds = dive->getStartTime().secondstime();
        uint32_t previousStepTimeInSeconds = startTimeDiveInSeconds;
        int16_t previousDepthInMeters = 0;
        for (DiveStep *step:dive->getSteps()) {
            int16_t stepEndDepthInMeters = DiveEquations::barToDepthInMeters(step->getPressureInBar());
            Serial.print(stepEndDepthInMeters);
            Serial.print(F("\t\t"));
            if (previousDepthInMeters == stepEndDepthInMeters) {
                char timeStr[9] = "";
                Serial.print(Formatter::formatTime(timeStr, step->getEndTime().secondstime() - previousStepTimeInSeconds, true));
                Serial.print(F("\t\t"));
            } else {
                Serial.print(F(" - "));
                Serial.print(F("\t\t\t"));
            }
            char timeStr[6] = "";
            Serial.print(Formatter::formatTimeInMinutes(timeStr, step->getEndTime().secondstime() - startTimeDiveInSeconds, Settings::MIN_STOP_TIME >= 60));
            Serial.print(F("\t\t"));
            Serial.println(step->getGasName());
            previousStepTimeInSeconds = step->getEndTime().secondstime();
            previousDepthInMeters = stepEndDepthInMeters;
        }
        Serial.println();
    }

    uint32_t time = dive->getDiveTimeInSeconds();
    for (DecompressionStep *stop:_stops) {
        time = stop->log(time);
    }
    Serial.println(F("=============================================="));
}


uint32_t DecompressionPlan::getTtsInSeconds() {
    double time = 0.0;
    for (DecompressionStep *stop:_stops) {
        time += stop->getDurationInSeconds();
    }
    return time;
}




