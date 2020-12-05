#include "GasManager.h"


Gas GasManager::AIR = Gas(0.21, "AIR");
Gas GasManager::NX32 = Gas(0.32, "NX32");
Gas GasManager::NX36 = Gas(0.36, "NX36");
Gas GasManager::NX40 = Gas(0.40, "NX40");
Gas GasManager::NX50 = Gas(0.50, "NX50");
Gas GasManager::TX18_35 = Gas(0.18, 0.35, "TX18_35");

void GasManager::init(FileSystem *fileSystem) {
    loadDefaultGasses(); // initialise with default gasses
    fileSystem->loadGasses(this); // override if gasses are stored. save defaults otherwise.
}

void GasManager::clear() {
    _ocGasses.clear();
    _ccGasses.clear();
}

void GasManager::addOcGas(Gas *divegas) {
    _ocGasses.push_back(divegas);
}

Gas *GasManager::getOcGas(char const *name) {
    return getGas(name, &_ocGasses);
}

Gas *GasManager::getBestOcGas(uint16_t depthInMeters) {
    return getBestGas(DiveEquations::depthInMetersToBars(depthInMeters), &_ocGasses);
}

Gas *GasManager::getCurrentOcGas() {
    return _currentOcGas;
}

Gas *GasManager::setCurrentOcGas(Gas *currentOcGas) {
    _currentOcGas = currentOcGas;
    _currentOcGas->setActive(true);
    return _currentOcGas;
}


void GasManager::addCcGas(Gas *divegas) {
    _ccGasses.push_back(divegas);
}

Gas *GasManager::getCcGas(char const *name) {
    return getGas(name, &_ccGasses);
}

Gas *GasManager::getBestCcGas(uint16_t depthInMeters) {
    return getBestGas(DiveEquations::depthInMetersToBars(depthInMeters), &_ccGasses);
}

Gas *GasManager::getCurrentCcGas() {
    return _currentOcGas;
}

Gas *GasManager::setCurrentCcGas(Gas *currentOcGas) {
    _currentOcGas = currentOcGas;
    _currentOcGas->setActive(true);
    return _currentOcGas;
}

std::list<Gas *> GasManager::getOcGasses() {
    return _ocGasses;
}

std::list<Gas *> GasManager::getCcGasses() {
    return _ccGasses;
}


Gas *GasManager::getGas(char const *name, std::list<Gas *> *gasList) {
    for (Gas *diveGas:*gasList) {
        if (strcmp(diveGas->getName(), name) == 0) {
            return diveGas;
        }
    }
    return nullptr;
}

Gas *GasManager::getBestGas(double pressureInBars, std::list<Gas *> *gasList) {
    Gas *bestGas = nullptr;
    for (Gas *diveGas:*gasList) {
        if (diveGas->isActive() && diveGas->isUsable(pressureInBars) &&
                (bestGas == nullptr || (diveGas->getO2() > bestGas->getO2()))) {
            bestGas = diveGas;
        }
    }
    return bestGas;
}

void GasManager::loadDefaultGasses() {
    clear();

    addOcGas(&GasManager::AIR);
    addOcGas(&GasManager::NX32);
    addOcGas(&GasManager::NX36);
    addOcGas(&GasManager::NX40);
    addOcGas(&GasManager::NX50);
    addOcGas(&GasManager::TX18_35);

    addCcGas(&GasManager::AIR);
    addCcGas(&GasManager::NX32);
    addCcGas(&GasManager::NX36);
    addCcGas(&GasManager::NX40);
    addCcGas(&GasManager::NX50);
    addCcGas(&GasManager::TX18_35);

    _currentOcGas = &AIR;
    _currentCcGas = &AIR;
}

size_t GasManager::serialize(File *file) {
    DynamicJsonDocument doc(getFileSize());

    doc["nrOfOcGasses"] = _ocGasses.size();
    doc["nrOfCcGasses"] = _ccGasses.size();
    JsonArray ocGassesJson = doc.createNestedArray("ocGasses");

    uint8_t i = 0;
    for (auto gas:_ocGasses) {
        ocGassesJson[i]["name"] = gas->getName();
        ocGassesJson[i]["o2"] = gas->getO2();
        ocGassesJson[i]["he"] = gas->getHe();
        ocGassesJson[i]["active"] = gas->isActive();
        i++;
    }
    JsonArray ccGassesJson = doc.createNestedArray("ccGasses");
    i = 0;
    for (auto gas:_ccGasses) {
        ccGassesJson[i]["name"] = gas->getName();
        ccGassesJson[i]["o2"] = gas->getO2();
        ccGassesJson[i]["he"] = gas->getHe();
        ocGassesJson[i]["active"] = gas->isActive();
        i++;
    }
    Serial.println(F("Saving gasses."));
    serializeJsonPretty(doc, Serial);
    Serial.println();
    return serializeJsonPretty(doc, *file);
}

DeserializationError GasManager::deserialize(File *file) {
    DynamicJsonDocument doc(getFileSize());

    DeserializationError error = deserializeJson(doc, *file);
    if (error) { // stop deserializing if json parse failed
        return error;
    }

    // clear existing gasses (if any)
    clear();

    uint8_t nrOfOcGasses = doc["nrOfOcGasses"];
    uint8_t nrOfCcGasses = doc["nrOfCcGasses"];
    JsonArray ocGassesJson = doc["ocGasses"];

    for (uint8_t i; i < nrOfOcGasses; i++) {
        Gas *ocGas = new Gas(ocGassesJson[i]["o2"], ocGassesJson[i]["he"], ocGassesJson[i]["name"], ocGassesJson[i]["active"]);
        addOcGas(ocGas);
        if (doc["current"]) {
            _currentOcGas = ocGas;
        }
    }

    JsonArray ccGassesJson = doc["ccGasses"];
    for (uint8_t i; i < nrOfCcGasses; i++) {
        Gas *ccGas = new Gas(ccGassesJson[i]["o2"], ccGassesJson[i]["he"], ccGassesJson[i]["name"], ccGassesJson[i]["active"]);
        addCcGas(ccGas);
        if (doc["current"]) {
            _currentCcGas = ccGas;
        }
    }

    Serial.print(F("[#oc gasses:"));
    Serial.print(getOcGasses().size());
    Serial.print(F("] [oc gasses:"));
    for (auto gas:getOcGasses()) {
        Serial.print(F(" "));
        Serial.print(F(gas->getName()));
        if (strcmp(_currentOcGas->getName(), gas->getName()) == 0) {
            Serial.print(F("(current)"));
        }
    }

    Serial.print(F("] [#cc gasses:"));
    Serial.print(getCcGasses().size());
    Serial.print(F("] [cc gasses:"));
    for (auto gas:getCcGasses()) {
        Serial.print(F(" "));
        Serial.print(F(gas->getName()));
        if (strcmp(_currentCcGas->getName(), gas->getName()) == 0) {
            Serial.print(F("(current)"));
        }
    }
    Serial.print(F("] - "));

    return error;
}

size_t GasManager::getFileSize() {
    return JSON_OBJECT_SIZE(4) + // 2 properties (nrOf oc/cc gasses) + 2 arrays
           JSON_ARRAY_SIZE(MAX_OC_GASSES) + MAX_OC_GASSES * JSON_OBJECT_SIZE(5) + // OC gasses array contains MAX_OC_GASSES elements and each element has 4 properties
           JSON_ARRAY_SIZE(MAX_CC_GASSES) + MAX_CC_GASSES * JSON_OBJECT_SIZE(5); // CC gasses array contains MAX_CC_GASSES elements and each element has 4 properties
}








