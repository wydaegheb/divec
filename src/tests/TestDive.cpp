#include "TestDive.h"


TestDive::TestDive(DecoManager *decoManager) {
    _decoManager = decoManager;
    _decoManager->getGasManager()->clear();
}

void TestDive::setGF(uint8_t lowGF, uint8_t highGF) {
    Settings::GF_HIGH = highGF / 100.0;
    Settings::GF_LOW = lowGF / 100.0;
}

void TestDive::addGas(Gas *gas) {
    gas->setActive(true);
    _decoManager->getGasManager()->addOcGas(gas);
}

void TestDive::addDiveLeg(uint16_t deptInMeter, uint16_t durationInMinutes) {
    _diveLegs.emplace_back(deptInMeter, durationInMinutes);
}

DecompressionPlan *TestDive::getDecoPlan() {
    uint32_t time = 0;
    double lastDepth = 0.0;
    _decoManager->reset(time);
    _decoManager->getDive()->start(time);
    for (auto leg:_diveLegs) {
        time += ((leg._depthInMeter - lastDepth) / 10.0) * 60;
        Serial.print(F("Adding descent time: "));
        Serial.println(time);
        _decoManager->update(time, DiveEquations::depthInMetersToBars(leg._depthInMeter), 20.0, true);
        time += leg._durationInMinutes * 60.0;
        _decoManager->update(time, DiveEquations::depthInMetersToBars(leg._depthInMeter), 20.0, true);
        lastDepth = leg._depthInMeter;
    }
    return _decoManager->getDecoPlan();
}




