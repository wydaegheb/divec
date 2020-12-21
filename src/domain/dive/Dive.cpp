
#include "Dive.h"

Dive &Dive::getInstance() {
    static Dive instance; // Guaranteed to be destroyed.
    // Instantiated on first use.
    return instance;
}

Dive::Dive() {
    init();
}

void Dive::init() {
    _started = false;
    _ended = false;
    _surfaced = false;
    _nrOfSteps = 0;
    _surfacedTimeInSeconds = 0;
    _currentDepthInMeter = 0.0;
    _avgDepthInMeter = 0.0;
    _maxDepthInMeter = 0.0;
    _minTemperatureInCelsius = 100;
    _maxTemperatureInCelsius = 0;
}

void Dive::start(uint32_t startTime) {
    init();
    _startTime = startTime;
    _lastTimeStamp = startTime;
}

void Dive::end() {
    _ended = true;
}

void *Dive::update(uint32_t time, Gas *gas, double pressureInBar, double tempInCelsius) {
    // add dive step, log it and update last time stamp
    _lastTimeStamp = time;

    // update maxDepth, avgDepth, maxTemp, minTemp
    _currentDepthInMeter = DiveEquations::barToDepthInMeters(pressureInBar);
    if (_maxDepthInMeter < _currentDepthInMeter) {
        _maxDepthInMeter = _currentDepthInMeter;
    }
    _avgDepthInMeter = _avgDepthInMeter + (_currentDepthInMeter - _avgDepthInMeter) / _nrOfSteps;
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
    return _currentDepthInMeter;
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
    doc["start_time"] = _startTime;
    doc["end_time"] = _lastTimeStamp;
    doc["avg_depth"] = _avgDepthInMeter;
    doc["max_depth"] = _maxDepthInMeter;
    doc["min_temp"] = _minTemperatureInCelsius;
    doc["max_temp"] = _maxTemperatureInCelsius;

    return doc;
}

void Dive::deserializeObject(JsonObject &doc) {
    init();

    _startTime = doc["start_time"];
    _lastTimeStamp = doc["end_time"];
    _avgDepthInMeter = doc["avg_depth"];
    _maxDepthInMeter = doc["max_depth"];
    _minTemperatureInCelsius = doc["min_temp"];
    _maxTemperatureInCelsius = doc["max_temp"];
}

size_t Dive::getJsonSize() {
    return JSON_OBJECT_SIZE(7); // 7 properties
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
























