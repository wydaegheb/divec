
#include "Dive.h"

Dive::Dive() {
    _started = false;
    _ended = false;
    _surfaced = false;
    clearEntries();
}

Dive::~Dive() {
    clearEntries();
}

void Dive::clearEntries() {
    for (DiveStep *entry:_steps) {
        delete entry;
    }
    _steps.clear();
}

void Dive::addStep(DiveStep *diveStep) {
    _steps.emplace_back(diveStep);
}

void Dive::start(uint32_t startTime) {
    _started = true;
    _ended = false;
    _surfaced = false;
    _startTime = startTime;
}

void Dive::end() {
    _started = false;
    _ended = true;
}

DiveStep *Dive::update(uint32_t time, Gas *gas, double pressureInBar, double tempInCelsius) {
    // add dive step, log it and update last time stamp
    auto *step = new DiveStep(time, gas, pressureInBar, tempInCelsius);
    if (!isSurfaced()) {
        addStep(step);
    }
    _lastTimeStamp = time;

    // update maxDepth, avgDepth, maxTemp, minTemp
    double currentDepth = DiveEquations::barToDepthInMeters(pressureInBar);
    if (_maxDepthInMeter < currentDepth) {
        _maxDepthInMeter = currentDepth;
    }
    _avgDepthInMeter = _avgDepthInMeter + (currentDepth - _avgDepthInMeter) / _steps.size();
    if (_maxTemperatureInCelsius < tempInCelsius) {
        _maxTemperatureInCelsius = tempInCelsius;
    }
    if (_minTemperatureInCelsius > tempInCelsius) {
        _minTemperatureInCelsius = tempInCelsius;
    }

    // check if surfaced or end of dive
    if (pressureInBar < (Settings::SURFACE_PRESSURE + Settings::END_OF_DIVE_PRESSURE)) {
        if (!_surfaced) {
            Serial.print(F(" - surfaced - time: "));
            Serial.println(time);
            _surfaced = true;
            _surfacedTimeInSeconds = time;
        } else if ((time - _surfacedTimeInSeconds) > Settings::END_OF_DIVE_DELAY) {
            Serial.print(F(" - dive ended. time on surface:"));
            Serial.println(time - _surfacedTimeInSeconds);
            end();
        }
    } else {
        Serial.println(F(" - diving (not on surface) "));
        _surfaced = false;
    }
    return step;
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

double Dive::getCurrentDepthInMeters() {
    if (_steps.empty()) {
        return Settings::SURFACE_PRESSURE;
    }
    return DiveEquations::barToDepthInMeters(_steps.back()->getPressureInBar());
}

uint32_t Dive::getDiveTimeInSeconds() const {
    if (isStarted() || isEnded()) {
        return (_lastTimeStamp - _startTime);
    }
    return 0;
}

uint32_t Dive::getStartTime() const {
    return _startTime;
}

std::list<DiveStep *> Dive::getSteps() {
    return _steps;
}

uint32_t Dive::getLastTimeStamp() const {
    return _lastTimeStamp;
}

uint16_t Dive::getAvgDepthInMeter() const {
    return _avgDepthInMeter;
}

uint16_t Dive::getMaxDepthInMeter() const {
    return _maxDepthInMeter;
}

int8_t Dive::getMinTemperatureInCelsius() const {
    return _minTemperatureInCelsius;
}

int8_t Dive::getMaxTemperatureInCelsius() const {
    return _maxTemperatureInCelsius;
}

JsonObject Dive::serializeObject(JsonObject &doc) {
    compressSteps(); // compress steps before saving (avoid going out of memory) - TODO: refactor to be able to stream the steps

    doc["start_time"] = _startTime;
    doc["end_time"] = _lastTimeStamp;
    doc["avg_depth"] = _avgDepthInMeter;
    doc["max_depth"] = _maxDepthInMeter;
    doc["min_temp"] = _minTemperatureInCelsius;
    doc["max_temp"] = _maxTemperatureInCelsius;
    doc["nr_of_steps"] = _steps.size();

    JsonArray stepJson = doc.createNestedArray("steps");
    uint16_t i = 0;
    for (auto step:_steps) {
        JsonObject stepJsonObject = stepJson[i].createNestedObject();
        stepJson[i] = step->serializeObject(stepJsonObject);
        i++;
    }

    return doc;
}

void Dive::deserializeObject(JsonObject &doc) {
    _started = false;
    _ended = true;
    _surfaced = true;
    clearEntries();

    _startTime = doc["start_time"];
    _lastTimeStamp = doc["end_time"];
    _avgDepthInMeter = doc["avg_depth"];
    _maxDepthInMeter = doc["max_depth"];
    _minTemperatureInCelsius = doc["min_temp"];
    _maxTemperatureInCelsius = doc["max_temp"];
    uint16_t nrOfSteps = doc["nr_of_steps"];

    JsonArray stepJson = doc["steps"];
    for (uint16_t i; i < nrOfSteps; i++) {
        auto *step = new DiveStep();
        JsonObject stepJsonObject = stepJson.getElement(i);
        step->deserializeObject(stepJsonObject);
        addStep(step);
    }
}

size_t Dive::getFileSize() {
    return JSON_OBJECT_SIZE(8) + // 7 properties + 1 array
           JSON_ARRAY_SIZE(MAX_NR_OF_STEPS) + MAX_NR_OF_STEPS * JSON_OBJECT_SIZE(4); // steps array contains MAX_NR_OF_STEPS elements and each element has 4 properties
}

void Dive::log() {
    log(&Serial);
}

void Dive::log(Print *print) {
    print->println(F("\n\n=============================================="));
    print->print(F("Dive - "));
    print->print(DateTime(_startTime).timestamp(DateTime::TIMESTAMP_DATE));
    print->print(" - ");
    print->println(DateTime(_startTime).timestamp(DateTime::TIMESTAMP_TIME));
    print->println(F("=============================================="));
    print->println(F("Time\t\t\tDepth\t\tMix\t\tTemp"));
    for (DiveStep *step:_steps) {
        step->log(print);
    }
    print->println(F("=============================================="));
    print->print(F("Dive time: "));
    TimeSpan timeSpan = TimeSpan(getDiveTimeInSeconds());
    print->print(timeSpan.hours());
    print->print(F(":"));
    print->print(timeSpan.minutes());
    print->print(F(":"));
    print->println(timeSpan.seconds());
    print->print(F("Max depth: "));
    print->println(_maxDepthInMeter);
    print->print(F("Avg depth: "));
    print->println(_avgDepthInMeter);
    print->print(F("Min temp: "));
    print->println(_minTemperatureInCelsius);
    print->print(F("Max temp: "));
    print->println(_maxTemperatureInCelsius);
    print->println(F("=============================================="));
}

// every step contains the end time and the end pressure
// therefore we can just delete all even numbered steps without breaking something.
// e.g.
// step 1: endTime: 3 sec, endPressure: 0.5 bar
// step 2: endTime: 6 sec, endPressure: 1.0 bar
// step 3: endTime: 9 sec, endPressure: 1.5 bar
// step 4: endTime: 12 sec, endPressure: 2.0 bar
// step 5: endTime: 15 sec, endPressure: 2.5 bar
// step 6: endTime: 18 sec, endPressure: 2.5 bar
// step 7: endTime: 21 sec, endPressure: 2.0 bar
// step 8: endTime: 24 sec, endPressure: 1.5 bar
// step 9: endTime: 27 sec, endPressure: 1.0 bar
// step 10: endTime: 30 sec, endPressure: 0.5 bar
// step 11: endTime: 33 sec, endPressure: 0 bar
//
// becomes
//
// step 1: endTime: 3 sec, endPressure: 0.5 bar
// step 3: endTime: 9 sec, endPressure: 1.5 bar
// step 5: endTime: 15 sec, endPressure: 2.5 bar
// step 7: endTime: 21 sec, endPressure: 2.0 bar
// step 9: endTime: 27 sec, endPressure: 1.0 bar
// step 11: endTime: 33 sec, endPressure: 0 bar
//
// which is a less accurate but still correct description of the dive. Only "resolution" is lost.
void Dive::compressSteps() {
    // steps size small enough -> do nothing
    // otherwise remove every even step
    while (_steps.size() > MAX_NR_OF_STEPS) {
        Serial.println(F(" - Compressing steps."));
        // remove every even step
        bool evenStep = false;

        auto it = _steps.begin();
        while (it != _steps.end()) {
            if (evenStep) {
                it = _steps.erase(it);
            }
            evenStep = !evenStep;
        }
    }
}

























