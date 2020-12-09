#include <RTClib.h>
#include "DiveStep.h"


DiveStep::DiveStep(const DateTime &endTime, Gas *gas, double pressureInBar, double temperatureInCelsius) {
    setEndTime(endTime);
    setGasName(gas);
    setPressureInBar(pressureInBar);
    setTemperatureInCelsius(temperatureInCelsius);
}



DateTime DiveStep::getEndTime() {
    return _endTime;
}

void DiveStep::setEndTime(const DateTime &endTime) {
    _endTime = endTime;
}

double DiveStep::getPressureInBar() const {
    return _pressureInBar;
}

void DiveStep::setPressureInBar(double pressureInBar) {
    _pressureInBar = pressureInBar;
}

double DiveStep::getTemperatureInCelsius() const {
    return _temperatureInCelsius;
}

void DiveStep::setTemperatureInCelsius(double temperatureInCelsius) {
    _temperatureInCelsius = temperatureInCelsius;
}

char const *DiveStep::getGasName() {
    return _gasName;
}

void DiveStep::setGasName(Gas *gas) {
    _gasName = gas->getName();
}

JsonObject DiveStep::serializeObject(JsonObject &doc) {
    doc["end_time"] = _endTime.secondstime();
    doc["pres"] = _pressureInBar;
    doc["temp"] = _temperatureInCelsius;
    doc["gas"] = _gasName;
    return doc;
}

size_t DiveStep::serialize(File *file) {
    DynamicJsonDocument doc(getFileSize());
    JsonObject docObject = doc.to<JsonObject>();
    doc = serializeObject(docObject);
/*    doc["end_time"] = _endTime.secondstime();
    doc["pres"] = _pressureInBar;
    doc["temp"] = _temperatureInCelsius;
    doc["gas"] = _gasName;*/

    return serializeJson(doc, *file);
}

void DiveStep::deserializeObject(JsonObject &doc) {
    _endTime = DateTime((uint32_t) doc["end_time"]);
    _pressureInBar = doc["pres"];
    _temperatureInCelsius = doc["temp"];
    _gasName = doc["gas"];
}

DeserializationError DiveStep::deserialize(File *file) {
    DynamicJsonDocument doc(getFileSize());

    DeserializationError error = deserializeJson(doc, *file);
    if (error) { // stop deserializing if json parse failed
        return error;
    }

    JsonObject docObject = doc.to<JsonObject>();
    deserializeObject(docObject);

/*    _endTime = DateTime((uint32_t) doc["end_time"]);
    _pressureInBar = doc["pres"];
    _temperatureInCelsius = doc["temp"];
    _gasName = doc["gas"];*/

    return error;
}

size_t DiveStep::getFileSize() {
    return JSON_OBJECT_SIZE(4) + BUFFER_FOR_STRINGS_DUPLICATION; // 4 properties
}

void DiveStep::log() {
    log(&Serial);
}

void DiveStep::log(Print *print) {
    print->print(_endTime.timestamp(DateTime::TIMESTAMP_TIME));
    print->print(F("\t\t"));
    print->print(DiveEquations::barToDepthInMeters(_pressureInBar));
    print->print(F("\t\t"));
    print->print(_gasName);
    print->print(F("\t\t"));
    print->println(_temperatureInCelsius);
}





