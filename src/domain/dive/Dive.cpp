
#include "Dive.h"

Dive::Dive(FileSystem *fileSystem) {
    _started = false;
    _ended = false;
    _surfaced = false;
    clearEntries();

    _logBook = new LogBook(fileSystem); // loads the existing logbook or creates a new one if it didn't exist
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

void Dive::start(const DateTime &startTime) {
    _started = true;
    _ended = false;
    _surfaced = false;
    _startTime = startTime;

    _logBook->clearTmpDiveLog();
}

void Dive::end() {
    _started = false;
    _ended = true;

    // close and update the dive log
    _logBook->saveDiveLog(this, getDiveTimeInSeconds(), _lastTimeStamp.secondstime(), getMaxDepthInMeter());
}

void Dive::update(const DateTime &time, Gas *gas, double pressureInBar, double tempInCelsius) {
    // add dive step, log it and update last time stamp
    auto *step = new DiveStep(time, gas, pressureInBar, tempInCelsius);
    addStep(step);
    _logBook->addDiveStep(step);
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
            Serial.println(time.secondstime());
            _surfaced = true;
            _surfacedTimeInSeconds = time.secondstime();
        } else if ((time.secondstime() - _surfacedTimeInSeconds) > Settings::END_OF_DIVE_DELAY) {
            Serial.print(F(" - dive ended. time on surface:"));
            Serial.println(time.secondstime() - _surfacedTimeInSeconds);
            end();
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

double Dive::getCurrentDepthInMeters() {
    if (_steps.empty()) {
        return Settings::SURFACE_PRESSURE;
    }
    return DiveEquations::barToDepthInMeters(_steps.back()->getPressureInBar());
}

uint32_t Dive::getDiveTimeInSeconds() const {
    if (!isStarted()) {
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

size_t Dive::serialize(File *file) {
    DynamicJsonDocument doc(getFileSize());

    doc["start_time"] = _startTime.secondstime();
    doc["end_time"] = _lastTimeStamp.secondstime();
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

    return serializeJsonPretty(doc, *file);
}

DeserializationError Dive::deserialize(File *file) {
    _started = false;
    _ended = true;
    _surfaced = true;
    clearEntries();

    DynamicJsonDocument doc(getFileSize());

    DeserializationError error = deserializeJson(doc, *file);
    if (error) { // stop deserializing if json parse failed
        return error;
    }

    _startTime = DateTime((uint32_t) doc["start_time"]);
    _lastTimeStamp = DateTime((uint32_t) doc["end_time"]);
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

    return error;
}

size_t Dive::getFileSize() {
    size_t stepsFileSize = 0;
    for (auto step:_steps) {
        stepsFileSize += step->getFileSize();
    }
    return JSON_OBJECT_SIZE(6) + stepsFileSize + BUFFER_FOR_STRINGS_DUPLICATION; // 6 properties + steps
}

void Dive::log() {
    log(&Serial);
}

void Dive::log(Print *print) {
    print->println(F("\n\n=============================================="));
    print->print(F("Dive - "));
    print->println(_startTime.timestamp());
    print->println(F("=============================================="));
    print->println(F("Time\t\t\tDepth\tMix\t\tTemp"));
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
    print->println(F("=============================================="));
}

























