#ifndef DIVEC_BUHLMANN_TISSUE_H
#define DIVEC_BUHLMANN_TISSUE_H

#include <decomanager/algorithms/equations/DiveEquations.h>
#include <decomanager/algorithms/buhlmann/BuhlmannTable.h>

class BuhlmannTissue {
public:

    BuhlmannTissue(BuhlmannTable buhlmannTable, int compartment);

    void update(uint32_t beginTimeInSeconds, uint32_t endTimeInSeconds, double beginPressureInBar, double endPressureInBar, double gasN2Fraction, double gasHeFraction);

    void addConstantDepthDiveStep(double pressureInBar, double gasN2Fraction, double gasHeFraction, uint16_t durationInSeconds);

    void addDepthChangingDiveStep(double endDepth, double speedInBar, double gasN2Fraction, double gasHeFraction, uint16_t durationInSeconds);

    double calculateCeilingInMeter(double gradientFactor) const;

    void resetTissue();

    void startSimulation();

    void undoSimulation();

    double getPN2() const;

    double getPHe() const;

    double getPTotal() const;

    void setPN2(double pN2);

    void setPHe(double pHe);

    void setPTotal(double pTotal);

private:
    double _pN2;
    double _pHe;
    double _pTotal;

    double _n2TimeConstant; // log(2.0) / n2HalfLife for this compartment
    double _n2AValue;
    double _n2BValue;

    double _heTimeConstant; // log(2.0) / n2HalfLife for this compartment
    double _heAValue;
    double _heBValue;

    // initial value before any gas loading
    double _initial_pN2;

    // temp storage while calculating decompressionplan/NDL/...
    double _pN2AtSimulationStart;
    double _pHeAtSimulationStart;
    double _pTotalAtSimulationStart;

};

#endif