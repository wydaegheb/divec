#include "DiveEquations.h"


double DiveEquations::schreinerEquation(double pBegin, double pGas, double durationInMinutes, double timeConstant, double gasRate) {
    return pGas + gasRate * (durationInMinutes - 1.0 / timeConstant) - (pGas - pBegin - gasRate / timeConstant) * exp(-timeConstant * durationInMinutes);
}

double DiveEquations::instantaneousEquation(double pBegin, double pGas, double durationInMinutes, double timeConstant) {
    return pBegin + (pGas - pBegin) * (1.0 - exp(-timeConstant * durationInMinutes));
}

double DiveEquations::gasPressureBreathingInBars(double pressureInBars, double fGas) {
    return (pressureInBars - Constants::WATER_VAPOR) * fGas;
}

double DiveEquations::gasPressureInBars(double pressureInBars, double fGas) {
    return pressureInBars * fGas;
}

double DiveEquations::gasCcrPressureBreathingInBars(double depth, double fGas, double fO2, double setPoint) {
    return ((depthInMetersToBars(depth) - setPoint) / (1.0 - fO2)) * fGas;
}

double
DiveEquations::gasRateInBarsPerMinute(double startPressureInBars, double endPressureInBars, double durationInMinutes, double fGas) {
    double speed = (endPressureInBars - startPressureInBars) / durationInMinutes;
    return speed * fGas;
}

double DiveEquations::depthInMetersToBars(double depthInMeters) {
    if (depthInMeters < 0) {
        return Settings::SURFACE_PRESSURE;
    }
    return (depthInMeters * Settings::getWaterPressure()) + Settings::SURFACE_PRESSURE;
}

double DiveEquations::barToDepthInMeters(double presureInBars) {
    if (presureInBars <= Settings::SURFACE_PRESSURE) {
        return 0.0;
    }
    return (presureInBars - Settings::SURFACE_PRESSURE) / Settings::getWaterPressure();
}

double DiveEquations::ccrSchreinerEquation(double pBegin, double pGas, double inertFraction, double o2Fraction, double halfTimeInMinutes, double durationInMinutes, double gasRate, double setPoint) {
    double timeConstant = log(2.0) / halfTimeInMinutes;
    double ekt = pow(2, -durationInMinutes / halfTimeInMinutes);
    double inertRatio = inertFraction / (1 - o2Fraction);
    return inertRatio * (ekt - 1.0) * (setPoint + (gasRate / timeConstant)) + (ekt * (pBegin - inertRatio * pBegin)) + inertRatio * pGas;
}