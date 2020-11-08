#ifndef DIVEC_BUHLMANN_TISSUE_H
#define DIVEC_BUHLMANN_TISSUE_H

#include <decomanager/algorithms/equations/DiveEquations.h>
#include "BuhlmannTable.h"

class BuhlmannTissue {
public:

    BuhlmannTissue(BuhlmannTable buhlmannTable, int compartment);

    void update(uint32_t beginTimeInSeconds, uint32_t endTimeInSeconds, double beginPressureInBar, double endPressureInBar, double gasN2Fraction, double gasHeFraction);

    void addConstantDepthDiveStep(double pressureInBar, double gasN2Fraction, double gasHeFraction, uint16_t durationInSeconds);

    void addDepthChangingDiveStep(double endDepth, double speedInBar, double gasN2Fraction, double gasHeFraction, uint16_t durationInSeconds);

    void addCcrConstantDepthDiveStep(double depth, double gasN2Fraction, double gasHeFraction, uint16_t durationInSeconds, double setPoint);

    void addCcrDepthChangingDiveStep(double startDepth, double endDepth, double gasN2Fraction, double gasHeFraction, uint16_t durationInSeconds, double setPoint);

    void startSimulation();

    void undoSimulation();

    uint16_t calculateCeilingInMeter(double gradientFactor) const;

    double getPN2() const;

    double getPHe() const;

    double getPTotal() const;

    void setPN2(double pN2);

    void setPHe(double pHe);

    void setPTotal(double pTotal);

private:
    uint8_t _compartment;
    double _pN2;
    double _pHe;
    double _pTotal;

    double _previousPN2;
    double _previousPHe;
    double _previousPTotal;

    double _pAtmosphere;
    double _n2HalfTime;
    double _n2AValue;
    double _n2BValue;

    double _heHalfTime;
    double _heAValue;
    double _heBValue;
};

#endif