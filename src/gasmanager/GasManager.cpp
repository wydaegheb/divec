#include "GasManager.h"


Gas GasManager::AIR = Gas(0.21, "AIR");
Gas GasManager::NX32 = Gas(0.32, "NX32");
Gas GasManager::NX36 = Gas(0.36, "NX36");
Gas GasManager::NX40 = Gas(0.40, "NX40");
Gas GasManager::NX50 = Gas(0.50, "NX50");
Gas GasManager::TX18_35 = Gas(0.18, 0.35, "TX18_35");

void GasManager::init(FileSystem *fileSystem) {
    loadDefaultGasses(); // initialise with default gasses
    fileSystem->loadGasses(this); // overrides default gasses if gas file exists. save defaults otherwise.
}

void GasManager::clear() {
    for (int i = 0; i < _nrOfGasses; i++) {
        delete _gasses[i];
        _gasses[i] = nullptr;
    }
    _nrOfGasses = 0;
    _currentGas = nullptr;
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
            (bestGas == nullptr || (_gasses[i]->getO2() > bestGas->getO2()))) {
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


void GasManager::loadDefaultGasses() {
    clear();

    addGas(&GasManager::AIR);
    addGas(&GasManager::NX32);
    addGas(&GasManager::NX36);
    addGas(&GasManager::NX40);
    addGas(&GasManager::NX50);
    addGas(&GasManager::TX18_35);

    _currentGas = &AIR;
}

JsonObject GasManager::serializeObject(JsonObject &doc) {
    doc["nrOfOcGasses"] = _nrOfGasses;
    JsonArray ocGassesJson = doc.createNestedArray("ocGasses");
    uint8_t i = 0;
    for (auto gas:_gasses) {
        JsonObject gasJsonObject = ocGassesJson[i].createNestedObject();
        ocGassesJson[i] = gas->serializeObject(gasJsonObject);
        i++;
    }

    return doc;
}

void GasManager::deserializeObject(JsonObject &doc) {
    // clear existing gasses (if any)
    clear();

    _nrOfGasses = doc["nrOfOcGasses"];

    JsonArray ocGassesJson = doc["ocGasses"];
    for (uint8_t i; i < _nrOfGasses; i++) {
        Gas *ocGas = new Gas();
        JsonObject gasJsonObject = ocGassesJson.getElement(i);
        ocGas->deserializeObject(gasJsonObject);
        addGas(ocGas);
    }
}

size_t GasManager::getJsonSize() {
    return JSON_OBJECT_SIZE(4) + // 1 properties (nrOf oc gasses) + 1 array
           JSON_ARRAY_SIZE(MAX_GASSES) + MAX_GASSES * JSON_OBJECT_SIZE(4); // gasses array contains MAX_GASSES elements and each element has 4 properties
}








