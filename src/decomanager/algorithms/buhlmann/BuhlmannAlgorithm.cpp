#include "BuhlmannAlgorithm.h"

BuhlmannAlgorithm::BuhlmannAlgorithm(char const* name, BuhlmannTable buhlmannTable) {
    _name = name;
    _buhlmannGasLoading = new BuhlmannGasLoading(buhlmannTable);
}

BuhlmannAlgorithm::~BuhlmannAlgorithm() {
    delete _buhlmannGasLoading;
    _buhlmannGasLoading = nullptr;
}

void BuhlmannAlgorithm::reset() {
    _buhlmannGasLoading->resetTissues();
}

void BuhlmannAlgorithm::update(uint32_t beginTimeInSeconds, uint32_t endTimeInSeconds, GasManager *gasManager, double beginPressureInBar, double endPressureInBar) {
    Gas *gas = gasManager->getCurrentOcGas();
    _buhlmannGasLoading->update(beginTimeInSeconds, endTimeInSeconds, beginPressureInBar, endPressureInBar, gas->getN2(), gas->getHe());
}

DecompressionPlan *BuhlmannAlgorithm::getDecoPlan(GasManager *gasManager) {
    return _buhlmannGasLoading->getDecoPlan(gasManager);
}

uint32_t BuhlmannAlgorithm::getNdlInSeconds(GasManager *gasManager) {
    return _buhlmannGasLoading->getNdlInSeconds(gasManager);
}

char const* BuhlmannAlgorithm::getName() {
    return _name;
}

JsonObject BuhlmannAlgorithm::serializeObject(JsonObject &algorithmJson) {
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

void BuhlmannAlgorithm::deserializeObject(JsonObject &algorithmJson) {
    _buhlmannGasLoading->setLastPressureInBar(algorithmJson["lastPressureInBars"]);
    uint8_t i = 0;
    for (auto tissue:_buhlmannGasLoading->getTissues()) {
        tissue->setPN2(algorithmJson["tissues"][i]["n2"]);
        tissue->setPHe(algorithmJson["tissues"][i]["he"]);
        tissue->setPTotal(algorithmJson["tissues"][i]["total"]);
        i++;
    }
}

size_t BuhlmannAlgorithm::getJsonSize() {
    return JSON_OBJECT_SIZE(2) + // lastPressureInBars property + array entry
           JSON_ARRAY_SIZE(_buhlmannGasLoading->getTissues().size()) +
           _buhlmannGasLoading->getTissues().size() * JSON_OBJECT_SIZE(3); // tissues array and each tissue has 3 properties
}



