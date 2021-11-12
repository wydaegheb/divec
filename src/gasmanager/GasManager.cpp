#include "GasManager.h"


Gas GasManager::AIR = Gas(21);

void GasManager::init(FileSystem *fileSystem) {
    loadDefaultGasses(); // initialise with default gasses
    fileSystem->loadGasses(this); // overrides default gasses if gas file exists. save defaults otherwise.
}

void GasManager::clear() {
    Serial.println(F("clear gasses"));
    for (int i = 0; i < _nrOfGasses; i++) {
        delete _gasses[i];
        _gasses[i] = nullptr;
    }
    _nrOfGasses = 0;
    _currentGas = nullptr;
    Serial.println(F("clear gasses done"));

}

void GasManager::addGas(Gas *divegas) {
    _gasses[_nrOfGasses] = divegas;
    _nrOfGasses++;
    if (_currentGas == nullptr) {
        _currentGas = divegas;
        _currentGas->setActive(true);
    }
}

Gas *GasManager::getGas(char const *name) {
    for (int i = 0; i < _nrOfGasses; i++) {
        if (strcmp(_gasses[i]->getName(), name) == 0) {
            return _gasses[i];
        }
    }
    return nullptr;
}

Gas *GasManager::getBestGas(uint16_t depthInMeters) {
    Gas *bestGas = nullptr;
    for (int i = 0; i < _nrOfGasses; i++) {
        if (_gasses[i]->isActive() && _gasses[i]->isUsable(DiveEquations::depthInMetersToBars(depthInMeters)) &&
            (bestGas == nullptr || (_gasses[i]->getO2frac() > bestGas->getO2frac()))) {
            bestGas = _gasses[i];
        }
    }

    return bestGas;
}

Gas *GasManager::getCurrentGas() {
    return _currentGas;
}

Gas *GasManager::setCurrentGas(Gas *currentGas) {
    _currentGas = currentGas;
    _currentGas->setActive(true);
    return _currentGas;
}

Gas *GasManager::getGas(int index) {
    return _gasses[index];
}

uint8_t GasManager::getNrOfGasses() const {
    return _nrOfGasses;
}


void GasManager::loadDefaultGasses() {
    clear();

    Gas *AIR = new Gas(21);
    addGas(AIR);
    addGas(new Gas(32));
    addGas(new Gas(36));
    addGas(new Gas(40));
    addGas(new Gas(50));
    addGas(new Gas(18, 35));

    _currentGas = AIR;
}

JsonObject GasManager::serializeObject(JsonObject &doc) {
    doc["nrOfOcGasses"] = _nrOfGasses;
    JsonArray ocGassesJson = doc.createNestedArray("ocGasses");
    for (uint8_t i = 0; i < _nrOfGasses; i++) {
        JsonObject gasJsonObject = ocGassesJson[i].createNestedObject();
        ocGassesJson[i] = _gasses[i]->serializeObject(gasJsonObject);
    }

    return doc;
}

void GasManager::deserializeObject(JsonObject &doc) {
    // clear existing gasses (if any)
    clear();

    uint8_t tmpNrOfGasses = doc["nrOfOcGasses"];

    JsonArray ocGassesJson = doc["ocGasses"];
    for (uint8_t i = 0; i < tmpNrOfGasses; i++) {
        Gas *ocGas = new Gas();
        JsonObject gasJsonObject = ocGassesJson.getElement(i);
        ocGas->deserializeObject(gasJsonObject);
        addGas(ocGas);

    }
}

size_t GasManager::getJsonSize() {
    return JSON_OBJECT_SIZE(4) + // 1 properties (nrOf oc gasses) + 1 array
           JSON_ARRAY_SIZE(MAX_GASSES) + MAX_GASSES * JSON_OBJECT_SIZE(
            4); // gasses array contains MAX_GASSES elements and each element has 4 properties
}














