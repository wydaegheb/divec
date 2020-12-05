
#include "Dive.h"

Dive::~Dive() {
    clearEntries();
}

void Dive::init(FileSystem *fileSystem) {
    _started = false;
    _ended = false;
    _surfaced = false;
    _logBook = new LogBook();
    fileSystem->loadLogbook(_logBook); // TODO: handle failure to open the file
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
    uint16_t currentDepth = DiveEquations::barToDepthInMeters(pressureInBar);
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

uint16_t Dive::getCurrentDepthInMeters() {
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

    doc["startTime"] = _startTime.secondstime();
    doc["lastTimeStamp"] = _lastTimeStamp.secondstime();
    doc["avgDepthInMeter"] = _avgDepthInMeter;
    doc["maxDepthInMeter"] = _maxDepthInMeter;
    doc["minTemperatureInCelsius"] = _minTemperatureInCelsius;
    doc["maxTemperatureInCelsius"] = _maxTemperatureInCelsius;
    doc["nrOfSteps"] = _steps.size();

    JsonArray stepJson = doc.createNestedArray("steps");
    uint16_t i = 0;
    for (auto step:_steps) {
        stepJson[i]["endTime"] = step->getEndTime().secondstime();
        stepJson[i]["lastTimeStamp"] = step->getPressureInBar();
        stepJson[i]["temperatureInCelsius"] = step->getTemperatureInCelsius();
        stepJson[i]["gasName"] = step->getGasName();
        i++;
    }

    return serializeJsonPretty(doc, *file);
}

DeserializationError Dive::deserialize(File *file) {
    DynamicJsonDocument doc(getFileSize());

    DeserializationError error = deserializeJson(doc, *file);
    if (error) { // stop deserializing if json parse failed
        return error;
    }

    _startTime = DateTime((uint32_t) doc["startTime"]);
    _lastTimeStamp = DateTime((uint32_t) doc["lastTimeStamp"]);
    _avgDepthInMeter = doc["avgDepthInMeter"];
    _maxDepthInMeter = doc["maxDepthInMeter"];
    _minTemperatureInCelsius = doc["minTemperatureInCelsius"];
    _maxTemperatureInCelsius = doc["maxTemperatureInCelsius"];
    uint16_t nrOfSteps = doc["nrOfSteps"];


    _started = false;
    _ended = true;
    _surfaced = true;
    clearEntries();
    for (uint16_t stepIdx; stepIdx < nrOfSteps; stepIdx++) {


    }

    return error;
}

size_t Dive::getFileSize() {
    return JSON_OBJECT_SIZE(6); // 6 properties
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























