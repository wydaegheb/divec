#include "Dive.h"

Dive::~Dive() {
    clearEntries();
}

void Dive::init() {
    _started = false;
    _ended = false;
    _surfaced = false;
    clearEntries();
}

void Dive::clearEntries() {
    for (DiveStep *entry:_steps) {
        delete entry;
    }
    _steps.clear();
}


void Dive::start(const DateTime& startTime) {
    _started = true;
    _ended = false;
    _surfaced = false;
    _startTime = startTime;
}

void Dive::update(const DateTime& time, Gas *gas, double pressureInBar, double tempInCelsius) {
    // add dive setp and update last time stamp
    _steps.emplace_back(new DiveStep(time, gas, pressureInBar, tempInCelsius));
    _lastTimeStamp = time;

    // check if surfaced or end of dive
    if (pressureInBar < (Settings::SURFACE_PRESSURE + Settings::END_OF_DIVE_PRESSURE)) {
        if (!_surfaced){
            Serial.print(F(" - surfaced - time: "));
            Serial.println(time.secondstime());
            _surfaced = true;
            _surfacedTimeInSeconds = time.secondstime();
        } else if ((time.secondstime() - _surfacedTimeInSeconds) > Settings::END_OF_DIVE_DELAY) {
            Serial.print(F(" - dive ended. time on surface:"));
            Serial.println(time.secondstime() - _surfacedTimeInSeconds);
            _ended = true;
            _started = false;
        }
    } else {
        Serial.println(F(" - diving (not on surface) "));
        _surfaced = false;
    }

}

bool Dive::isStarted() const {
    return _started;
}

bool Dive::isSurfaced() const {
    return _surfaced;
}

bool Dive::isEnded() const {
    return _ended;
}

bool Dive::isInProgress() const {
    return (isStarted() && !isEnded());
}

uint16_t Dive::getCurrentDepthInMeters() {
    if (_steps.empty()){
        return Settings::SURFACE_PRESSURE;
    }
    return DiveEquations::barToDepthInMeters(_steps.back()->getPressureInBar());
}

uint32_t Dive::getDiveTimeInSeconds() const {
    if (!isStarted()){
        return 0;
    }
    return (_lastTimeStamp.secondstime() - _startTime.secondstime());
}

const DateTime &Dive::getStartTime() const {
    return _startTime;
}

std::list<DiveStep *> Dive::getSteps() {
    return _steps;
}

const DateTime &Dive::getLastTimeStamp() const {
    return _lastTimeStamp;
}

void Dive::log() {
    Serial.println(F("\n\n=============================================="));
    Serial.print(F("Dive - "));
    Serial.println(_startTime.timestamp());
    Serial.println(F("=============================================="));
    Serial.println(F("Time\t\t\tDepth\tMix\t\tTemp"));
    for (DiveStep* step:_steps) {
        step->log();
    }
    Serial.println(F("=============================================="));
    Serial.print(F("Dive time: "));
    TimeSpan timeSpan = TimeSpan(getDiveTimeInSeconds());
    Serial.print(timeSpan.hours());
    Serial.print(F(":"));
    Serial.print(timeSpan.minutes());
    Serial.print(F(":"));
    Serial.println(timeSpan.seconds());
    Serial.println(F("=============================================="));
}


















