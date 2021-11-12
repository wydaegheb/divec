#include "LogBook.h"

void LogBook::init(FileSystem *fileSystem) {
    _fileSystem = fileSystem;
    _fileSystem->loadLogbook(this);
}

Dive *LogBook::loadDive(uint16_t diveNr) {
    Dive *dive = &Dive::getInstance();
    _fileSystem->loadDiveLog(dive, diveNr);
    return dive;
}

void LogBook::initTmpDiveLog() {
    _fileSystem->removeTmpDiveLog();
}


void LogBook::addDiveStep(uint32_t endTimeInSeconds, double pressureInBar, double temperatureInCelsius,
                          const char *gasName) {
    DynamicJsonDocument doc(JSON_OBJECT_SIZE(4) + 50);
    doc["end_time"] = endTimeInSeconds;
    doc["pres"] = pressureInBar;
    doc["temp"] = temperatureInCelsius;
    doc["gas"] = gasName;
    JsonObject docObject = doc.to<JsonObject>();

    _fileSystem->saveDiveLogStep(docObject);
}

uint16_t LogBook::getNumberOfDives() const {
    return _numberOfDives;
}

void LogBook::setNumberOfDives(uint16_t numberOfDives) {
    _numberOfDives = numberOfDives;
}

double LogBook::getMaximumDepth() const {
    return _maximumDepth;
}

void LogBook::setMaximumDepth(double maximumDepth) {
    _maximumDepth = maximumDepth;
}

uint32_t LogBook::getTotalDiveTime() const {
    return _totalDiveTime;
}

void LogBook::setTotalDiveTime(uint32_t totalDiveTime) {
    _totalDiveTime = totalDiveTime;
}

uint32_t LogBook::getLastDiveDate() const {
    return _lastDiveDate;
}

void LogBook::setLastDiveDate(uint32_t lastDiveDate) {
    _lastDiveDate = lastDiveDate;
}

void LogBook::saveDive(Dive *dive) {
    // update logbook
    _numberOfDives++;
    _totalDiveTime += dive->getDiveTimeInSeconds();
    _lastDiveDate = dive->getLastTimeStamp();
    if (_maximumDepth < dive->getMaxDepthInMeter()) {
        _maximumDepth = dive->getMaxDepthInMeter();
    }
    _fileSystem->saveLogbook(this);

    // save dive
    _fileSystem->saveDiveLog(dive, _numberOfDives);

    // clear tmp log (not needed? as we just renamed it it should not exist anymore.)
    _fileSystem->removeTmpDiveLog();

}

JsonObject LogBook::serializeObject(JsonObject &doc) {
    doc["numberOfDives"] = _numberOfDives;
    doc["maximumDepth"] = _maximumDepth;
    doc["totalDiveTime"] = _totalDiveTime;
    doc["lastDiveDate"] = _lastDiveDate;

    return doc;
}

void LogBook::deserializeObject(JsonObject &doc) {
    _numberOfDives = doc["numberOfDives"];
    _maximumDepth = doc["maximumDepth"];
    _totalDiveTime = doc["totalDiveTime"];
    _lastDiveDate = doc["lastDiveDate"];
}

size_t LogBook::getJsonSize() {
    return JSON_OBJECT_SIZE(4); // 4 properties
}














