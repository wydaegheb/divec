#include "BuhlmannAlgorithm.h"

BuhlmannAlgorithm::BuhlmannAlgorithm(BuhlmannTable buhlmannTable) {
    _buhlmannGasLoading = new BuhlmannGasLoading(buhlmannTable);
}

BuhlmannAlgorithm::~BuhlmannAlgorithm() {
    delete _buhlmannGasLoading;
    _buhlmannGasLoading = nullptr;
}

void BuhlmannAlgorithm::reset() {
    _buhlmannGasLoading->resetTissues();
}

void BuhlmannAlgorithm::applySurfaceInterval(uint32_t duration) {
    _buhlmannGasLoading->applySurfaceInterval(duration);
}

void BuhlmannAlgorithm::update(uint32_t beginTimeInSeconds, uint32_t endTimeInSeconds, Gas *gas, double beginPressureInBar, double endPressureInBar) {
    _buhlmannGasLoading->update(beginTimeInSeconds, endTimeInSeconds, beginPressureInBar, endPressureInBar, gas->getN2(), gas->getHe());
}

DecompressionPlan *BuhlmannAlgorithm::getDecoPlan(GasManager *gasManager, uint32_t runtimeInSeconds) {
    return _buhlmannGasLoading->getDecoPlan(gasManager, runtimeInSeconds);
}

uint32_t BuhlmannAlgorithm::getNdlInSeconds(GasManager *gasManager) {
    return _buhlmannGasLoading->getNdlInSeconds(gasManager);
}

JsonObject BuhlmannAlgorithm::serializeObject(JsonObject &algorithmJson) {
    algorithmJson["lastPressureInBars"] = _buhlmannGasLoading->getLastPressureInBar();
    JsonArray tissueJson = algorithmJson.createNestedArray("tissues");
    for (uint8_t i = 0; i < 16; i++) {
        tissueJson[i]["n2"] = _buhlmannGasLoading->getTissues()[i]->getPN2();
        tissueJson[i]["he"] = _buhlmannGasLoading->getTissues()[i]->getPHe();
        tissueJson[i]["total"] = _buhlmannGasLoading->getTissues()[i]->getPTotal();
    }
    return algorithmJson;
}

void BuhlmannAlgorithm::deserializeObject(JsonObject &algorithmJson) {
    _buhlmannGasLoading->setLastPressureInBar(algorithmJson["lastPressureInBars"]);
    for (uint8_t i = 0; i < 16; i++) {
        _buhlmannGasLoading->getTissues()[i]->setPN2(algorithmJson["tissues"][i]["n2"]);
        _buhlmannGasLoading->getTissues()[i]->setPHe(algorithmJson["tissues"][i]["he"]);
        _buhlmannGasLoading->getTissues()[i]->setPTotal(algorithmJson["tissues"][i]["total"]);
    }
}

size_t BuhlmannAlgorithm::getJsonSize() {
    return JSON_OBJECT_SIZE(2) + // lastPressureInBars property + array entry
           JSON_ARRAY_SIZE(16) + 16 * JSON_OBJECT_SIZE(3); // tissues array and each tissue has 3 properties
}





