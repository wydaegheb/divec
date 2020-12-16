#include "BuhlmannTissue.h"


BuhlmannTissue::BuhlmannTissue(BuhlmannTable buhlmannTable, int compartment) {
    resetTissue();

    _n2TimeConstant = log(2.0) / buhlmannTable.getN2HalfLife(compartment);
    _n2AValue = buhlmannTable.getN2A(compartment);
    _n2BValue = buhlmannTable.getN2B(compartment);

    _heTimeConstant = log(2.0) / buhlmannTable.getHeHalfLife(compartment);
    _heAValue = buhlmannTable.getHeA(compartment);
    _heBValue = buhlmannTable.getHeB(compartment);
}

void BuhlmannTissue::resetTissue() {
    _pN2 = DiveEquations::gasPressureBreathingInBars(Settings::SURFACE_PRESSURE, 0.79);
    _pHe = 0.0;
    _pTotal = _pN2 + _pHe;
}

void BuhlmannTissue::update(uint32_t beginTimeInSeconds, uint32_t endTimeInSeconds, double beginPressureInBar, double endPressureInBar, double gasN2Fraction, double gasHeFraction) {
    if (endPressureInBar == beginPressureInBar) { // flat
        addConstantDepthDiveStep(endPressureInBar, gasN2Fraction, gasHeFraction, endTimeInSeconds - beginTimeInSeconds);
    } else { // Ascent or Descent
        addDepthChangingDiveStep(beginPressureInBar, endPressureInBar, gasN2Fraction, gasHeFraction, endTimeInSeconds - beginTimeInSeconds);
    }
}

void BuhlmannTissue::addConstantDepthDiveStep(double pressureInBar, double gasN2Fraction, double gasHeFraction, uint16_t durationInSeconds) {
    double durationInMinutes = durationInSeconds / 60.0;
    //Calculate nitrogen loading
    double pGasN2 = DiveEquations::gasPressureBreathingInBars(pressureInBar, gasN2Fraction); // initial ambient N2 pressure
    _pN2 = DiveEquations::instantaneousEquation(_pN2, pGasN2, durationInMinutes, _n2TimeConstant);

    //Calculate helium Loading
    double pGasHe = DiveEquations::gasPressureBreathingInBars(pressureInBar, gasHeFraction); // initial ambient He pressure
    _pHe = DiveEquations::instantaneousEquation(_pHe, pGasHe, durationInMinutes, _heTimeConstant);

    //Calculate total loading
    _pTotal = _pN2 + _pHe;
}

void BuhlmannTissue::addDepthChangingDiveStep(double startPressureInBars, double endPressureInBars, double gasN2Fraction, double gasHeFraction, uint16_t durationInSeconds) {
    double durationInMinutes = durationInSeconds / 60.0;
    //Calculate nitrogen loading
    double gasRateN2 = DiveEquations::gasRateInBarsPerMinute(startPressureInBars, endPressureInBars, durationInMinutes, gasN2Fraction);
    double pGasN2 = DiveEquations::gasPressureBreathingInBars(startPressureInBars, gasN2Fraction); // initial ambient N2 pressure

    _pN2 = DiveEquations::schreinerEquation(_pN2, pGasN2, durationInMinutes, _n2TimeConstant, gasRateN2);

    //Calculate helium Loading
    double gasRateHe = DiveEquations::gasRateInBarsPerMinute(startPressureInBars, endPressureInBars, durationInMinutes, gasHeFraction);
    double pGasHe = DiveEquations::gasPressureBreathingInBars(startPressureInBars, gasHeFraction); // initial ambient He pressure

    _pHe = DiveEquations::schreinerEquation(_pHe, pGasHe, durationInMinutes, _heTimeConstant, gasRateHe);

    //Calculate total loading
    _pTotal = _pN2 + _pHe;
}


double BuhlmannTissue::calculateCeilingInMeter(double gradientFactor) const {
    double a = ((_n2AValue * _pN2) + (_heAValue * _pHe)) / _pTotal; //calculate a
    double b = ((_n2BValue * _pN2) + (_heBValue * _pHe)) / _pTotal; //calculate b
    double ceilingInBar = (_pTotal - a * gradientFactor) / (gradientFactor / b - gradientFactor + 1.0);

    if (ceilingInBar < 0.0) {
        ceilingInBar = 0.0;
    }

    return DiveEquations::barToDepthInMeters(ceilingInBar);
}

// Used to "simulate" the effect of decompression stops/ndl calculation on gas loading. Store current state to be able to undo the simulation
void BuhlmannTissue::startSimulation() {
    _pN2AtSimulationStart = _pN2;
    _pHeAtSimulationStart = _pHe;
    _pTotalAtSimulationStart = _pTotal;
}

// Used to "simulate" the effect of decompression stops/ndl calculation on gas loading. Restores the tissues once the calculation has finished
void BuhlmannTissue::undoSimulation() {
    // restore state from the beginning of the simulation
    _pN2 = _pN2AtSimulationStart;
    _pHe = _pHeAtSimulationStart;
    _pTotal = _pTotalAtSimulationStart;
}

double BuhlmannTissue::getPN2() const {
    return _pN2;
}

double BuhlmannTissue::getPHe() const {
    return _pHe;
}

double BuhlmannTissue::getPTotal() const {
    return _pTotal;
}

void BuhlmannTissue::setPN2(double pN2) {
    _pN2 = pN2;
}

void BuhlmannTissue::setPHe(double pHe) {
    _pHe = pHe;
}

void BuhlmannTissue::setPTotal(double pTotal) {
    _pTotal = pTotal;
}















