#include "BuhlmannAlgorithm.h"

BuhlmannAlgorithm::BuhlmannAlgorithm(char const* name, BuhlmannTable buhlmannTable) {
    _name = name;
    _buhlmannGasLoading = new BuhlmannGasLoading(buhlmannTable);
}

BuhlmannAlgorithm::~BuhlmannAlgorithm() {
    delete _buhlmannGasLoading;
    _buhlmannGasLoading = nullptr;
}


void BuhlmannAlgorithm::update(uint32_t beginTimeInSeconds, uint32_t endTimeInSeconds, GasManager *gasManager, double beginPressureInBar, double endPressureInBar) {
    Gas *gas = gasManager->getCurrentOcGas();
    _buhlmannGasLoading->update(beginTimeInSeconds, endTimeInSeconds, beginPressureInBar, endPressureInBar, gas->getN2(), gas->getHe());
    _decoPlan = _buhlmannGasLoading->getDecoPlan(gasManager);
}

DecompressionPlan *BuhlmannAlgorithm::getDecoPlan(GasManager *gasManager) {
    if (_decoPlan == nullptr) {
        _decoPlan = _buhlmannGasLoading->getDecoPlan(gasManager);
    }
    return _decoPlan;
}

uint32_t BuhlmannAlgorithm::getNdlInSeconds(GasManager *gasManager) {
    return _buhlmannGasLoading->getNdlInSeconds(gasManager);
}

char const* BuhlmannAlgorithm::getName() {
    return _name;
}

JsonObject BuhlmannAlgorithm::serialize(JsonObject &algorithmJson) {
    algorithmJson["lastPressureInBars"] = _buhlmannGasLoading->getLastPressureInBar();
    JsonArray tissueJson = algorithmJson.createNestedArray("tissues");
    uint8_t i = 0;
    for (auto tissue:_buhlmannGasLoading->getTissues()) {
        tissueJson[i]["n2"] = tissue->getPN2();
        tissueJson[i]["he"] = tissue->getPHe();
        tissueJson[i]["total"] = tissue->getPTotal();
        i++;
    }
    return algorithmJson;
}

void BuhlmannAlgorithm::deserialize(JsonObject &algorithmJson) {
    _buhlmannGasLoading->setLastPressureInBar(algorithmJson["lastPressureInBars"]);
    uint8_t i = 0;
    for (auto tissue:_buhlmannGasLoading->getTissues()) {
        tissue->setPN2(algorithmJson["tissues"][i]["n2"]);
        tissue->setPHe(algorithmJson["tissues"][i]["he"]);
        tissue->setPTotal(algorithmJson["tissues"][i]["total"]);
        i++;
    }
}

size_t BuhlmannAlgorithm::getObjectSize() {
    return JSON_OBJECT_SIZE(2) + // lastPressureInBars property + array entry
           JSON_ARRAY_SIZE(_buhlmannGasLoading->getTissues().size()) +
           _buhlmannGasLoading->getTissues().size() * JSON_OBJECT_SIZE(3) + // tissues array and each tissue has 3 properties
           39; // Additional bytes for strings duplication
}

