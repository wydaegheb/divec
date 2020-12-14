#ifndef DIVEC_TESTDIVE_H
#define DIVEC_TESTDIVE_H

#include <Arduino.h>
#include <decomanager/DecoManager.h>

struct DiveLeg {
    DiveLeg(double depthInMeter, double durationInMinutes) {
        _depthInMeter = depthInMeter;
        _durationInMinutes = durationInMinutes;
    }

    double _depthInMeter;
    double _durationInMinutes;
};

class TestDive {

public:

    explicit TestDive(DecoManager *decoManager);

    void setGF(uint8_t lowGF, uint8_t highGF);

    void addGas(Gas *gas);

    void addDiveLeg(uint16_t deptInMeter, uint16_t durationInMinutes);

    DecompressionPlan *getDecoPlan();

private:
    DecoManager *_decoManager;
    std::list<DiveLeg> _diveLegs;

};


#endif //DIVEC_TESTDIVE_H
