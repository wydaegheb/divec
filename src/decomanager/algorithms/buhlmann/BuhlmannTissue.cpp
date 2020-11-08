#include "BuhlmannTissue.h"


BuhlmannTissue::BuhlmannTissue(BuhlmannTable buhlmannTable, int compartment) {
    _compartment = compartment;
    _pAtmosphere = 1.0;
    _pN2 = 0.79 * _pAtmosphere - Constants::WATER_VAPOR;
    _pHe = 0.0;
    _pTotal = _pN2 + _pHe;

    _n2HalfTime = buhlmannTable.getN2HalfLife(compartment);
    _n2AValue = buhlmannTable.getN2A(compartment);
    _n2BValue = buhlmannTable.getN2B(compartment);
    _heHalfTime = buhlmannTable.getHeHalfLife(compartment);
    _heAValue = buhlmannTable.getHeA(compartment);
    _heBValue = buhlmannTable.getHeB(compartment);
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
    _pN2 = DiveEquations::instantaneousEquation(_pN2, pGasN2, durationInMinutes, _n2HalfTime);

    //Calculate helium Loading
    double pGasHe = DiveEquations::gasPressureBreathingInBars(pressureInBar, gasHeFraction); // initial ambient He pressure
    _pHe = DiveEquations::instantaneousEquation(_pHe, pGasHe, durationInMinutes, _heHalfTime);

/*    Serial.print(F("ADDCONSTANTDEPTHDIVESTEP ["));
    Serial.print(_compartment+1);
    Serial.print(F("] pressureInBar:"));
    Serial.print(pressureInBar);
    Serial.print(F(", gasN2Fraction:"));
    Serial.print(gasN2Fraction);
    Serial.print(F(", gasHeFraction:"));
    Serial.print(gasHeFraction);
    Serial.print(F(", durationInSeconds:"));
    Serial.print(durationInSeconds);
    Serial.print(F(", pGasN2:"));
    Serial.print(pGasN2);
    Serial.print(F(", _pN2:"));
    Serial.print(_pN2);
    Serial.print(F(", pGasHe:"));
    Serial.print(pGasHe);
    Serial.print(F(", _n2HalfTime:"));
    Serial.print(_n2HalfTime);
    Serial.print(F(", _heHalfTime:"));
    Serial.println(_heHalfTime);*/

    //Calculate total loading
    _pTotal = _pN2 + _pHe;
}

void BuhlmannTissue::addDepthChangingDiveStep(double startPressureInBars, double endPressureInBars, double gasN2Fraction, double gasHeFraction, uint16_t durationInSeconds) {
    double durationInMinutes = durationInSeconds / 60.0;
    //Calculate nitrogen loading
    double gasRateN2 = DiveEquations::gasRateInBarsPerMinute(startPressureInBars, endPressureInBars, durationInMinutes, gasN2Fraction);
    double pGasN2 = DiveEquations::gasPressureBreathingInBars(startPressureInBars, gasN2Fraction); // initial ambient N2 pressure

    _pN2 = DiveEquations::schreinerEquation(_pN2, pGasN2, durationInMinutes, _n2HalfTime, gasRateN2);

    //Calculate helium Loading
    double gasRateHe = DiveEquations::gasRateInBarsPerMinute(startPressureInBars, endPressureInBars, durationInMinutes, gasHeFraction);
    double pGasHe = DiveEquations::gasPressureBreathingInBars(startPressureInBars, gasHeFraction); // initial ambient He pressure

    _pHe = DiveEquations::schreinerEquation(_pHe, pGasHe, durationInMinutes, _heHalfTime, gasRateHe);

 /*   Serial.print(F("ADDDEPTHCHANGINGDIVESTEP ["));
    Serial.print(_compartment+1);
    Serial.print(F("] startPressureInBars:"));
    Serial.print(startPressureInBars);
    Serial.print(F(", endPressureInBars:"));
    Serial.print(endPressureInBars);
    Serial.print(F(", gasN2Fraction:"));
    Serial.print(gasN2Fraction);
    Serial.print(F(", gasHeFraction:"));
    Serial.print(gasHeFraction);
    Serial.print(F(", durationInSeconds:"));
    Serial.print(durationInSeconds);
    Serial.print(F(", gasRateN2:"));
    Serial.print(gasRateN2);
    Serial.print(F(", pGasN2:"));
    Serial.print(pGasN2);
    Serial.print(F(", _pN2:"));
    Serial.print(_pN2);
    Serial.print(F(", gasRateHe:"));
    Serial.print(gasRateHe);
    Serial.print(F(", pGasHe:"));
    Serial.print(pGasHe);
    Serial.print(F(", _pHe:"));
    Serial.print(_pHe);
    Serial.print(F(", _n2HalfTime:"));
    Serial.print(_n2HalfTime);
    Serial.print(F(", _heHalfTime:"));
    Serial.println(_heHalfTime);*/

    //Calculate total loading
    _pTotal = _pN2 + _pHe;
}

void BuhlmannTissue::addCcrConstantDepthDiveStep(double depth, double gasN2Fraction, double gasHeFraction, uint16_t durationInSeconds, double setPoint) {
    double durationInMinutes = durationInSeconds / 60.0;

    double o2Fraction = 1.0 - gasN2Fraction - gasHeFraction;

    //Calculate nitrogen loading
    double pGasN2 = DiveEquations::gasCcrPressureBreathingInBars(depth, gasN2Fraction, o2Fraction, setPoint); // initial ambient N2 pressure
    _pN2 = DiveEquations::instantaneousEquation(_pN2, pGasN2, durationInMinutes, _n2HalfTime);

    //Calculate helium Loading
    double pGasHe = DiveEquations::gasCcrPressureBreathingInBars(depth, gasHeFraction, o2Fraction, setPoint); // initial ambient He pressure
    _pHe = DiveEquations::instantaneousEquation(_pHe, pGasHe, durationInMinutes, _heHalfTime);

    //Calculate total loading
    _pTotal = _pN2 + _pHe;
}

void BuhlmannTissue::addCcrDepthChangingDiveStep(double startDepth, double endDepth, double gasN2Fraction, double gasHeFraction, uint16_t durationInSeconds, double setPoint) {
    double durationInMinutes = durationInSeconds / 60.0;

    double o2Fraction = 1.0 - gasN2Fraction - gasHeFraction;

    //Calculate nitrogen loading
    double gasRateN2 = DiveEquations::gasRateInBarsPerMinute(startDepth, endDepth, durationInMinutes, gasN2Fraction);
    double pGasN2 = DiveEquations::gasCcrPressureBreathingInBars(startDepth, gasN2Fraction, o2Fraction, setPoint); // initial ambient N2 pressure

    _pN2 = DiveEquations::schreinerEquation(_pN2, pGasN2, durationInMinutes, _n2HalfTime, gasRateN2);

    //Calculate helium Loading
    double gasRateHe = DiveEquations::gasRateInBarsPerMinute(startDepth, endDepth, durationInMinutes, gasHeFraction);
    double pGasHe = DiveEquations::gasCcrPressureBreathingInBars(startDepth, gasN2Fraction, o2Fraction, setPoint); // initial ambient He pressure

    _pHe = DiveEquations::schreinerEquation(_pHe, pGasHe, durationInMinutes, _heHalfTime, gasRateHe);

    //Calculate total loading
    _pTotal = _pN2 + _pHe;

/*    //Calculate nitrogen loading
    _pN2 = DiveEquations::ccrSchreinerEquation(_pN2, gasN2Fraction, o2Fraction, _n2HalfTime, duration, startDepth, endDepth, setPoint);

    //Calculate helium Loading
    _pN2 = DiveEquations::ccrSchreinerEquation(_pN2, gasHeFraction, o2Fraction, _heHalfTime, duration, startDepth, endDepth, setPoint);

    //Calculate total loading
    _pTotal = _pN2 + _pHe;*/
}


uint16_t BuhlmannTissue::calculateCeilingInMeter(double gradientFactor) const {
    double a = ((_n2AValue * _pN2) + (_heAValue * _pHe)) / _pTotal; //calculate a
    double b = ((_n2BValue * _pN2) + (_heBValue * _pHe)) / _pTotal; //calculate b
    double ceilingInBar = (_pTotal - a * gradientFactor) / (gradientFactor / b - gradientFactor + 1.0 );

    return DiveEquations::barToDepthInMeters(ceilingInBar);
}

// Used to "simulate" the effect of decompression stops on gas loading. Store current state to be able to undo the simulation
void BuhlmannTissue::startSimulation() {
    _previousPN2 = _pN2;
    _previousPHe = _pHe;
    _previousPTotal = _pTotal;
}

// Used to "simulate" the effect of decompression stops on gas loading. Restores the tissues once the decoplan has been calculated
void BuhlmannTissue::undoSimulation() {
    // restore state from the beginning of the simulation
    _pN2 = _previousPN2;
    _pHe = _previousPHe;
    _pTotal = _previousPTotal;
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











