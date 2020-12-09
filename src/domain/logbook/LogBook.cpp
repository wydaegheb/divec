#include "LogBook.h"

LogBook::LogBook(FileSystem *fileSystem) {
    _fileSystem = fileSystem;
    _fileSystem->loadLogbook(this);
}

void LogBook::clearTmpDiveLog() {
    _fileSystem->clearTmpDiveLog();
}


void LogBook::addDiveStep(JsonSerializable *step) {
    _fileSystem->writeTmpDiveLogStep(step);
}

void LogBook::saveDiveLog(JsonSerializable *dive, uint32_t diveTimeInSeconds, uint32_t lastDiveDate, uint16_t diveMaxDepth) {
    // update logbook
    _numberOfDives++;
    _totalDiveTime += diveTimeInSeconds;
    _lastDiveDate = lastDiveDate;
    if (diveMaxDepth > _maximumDepth) {
        _maximumDepth = diveMaxDepth;
    }
    _fileSystem->saveLogbook(this);

    // save dive
    _fileSystem->saveDiveLog(dive, _numberOfDives);

    // clear tmp log
    _fileSystem->clearTmpDiveLog();

}

size_t LogBook::serialize(File *file) {
    DynamicJsonDocument doc(getFileSize());

    doc["numberOfDives"] = _numberOfDives;
    doc["maximumDepth"] = _maximumDepth;
    doc["totalDiveTime"] = _totalDiveTime;
    doc["lastDiveDate"] = _lastDiveDate;

    return serializeJsonPretty(doc, *file);
}

DeserializationError LogBook::deserialize(File *file) {
    DynamicJsonDocument doc(getFileSize());

    DeserializationError error = deserializeJson(doc, *file);
    if (error) { // stop deserializing if json parse failed
        return error;
    }

    _numberOfDives = doc["numberOfDives"];
    _maximumDepth = doc["maximumDepth"];
    _lastDiveDate = doc["lastDiveDate"];
    _totalDiveTime = doc["totalDiveTime"];

    return error;
}

size_t LogBook::getFileSize() {
    return JSON_OBJECT_SIZE(4) + BUFFER_FOR_STRINGS_DUPLICATION; // 4 properties
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










