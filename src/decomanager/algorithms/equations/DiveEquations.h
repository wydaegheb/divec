#ifndef DIVEC_DIVE_EQUATIONS_H
#define DIVEC_DIVE_EQUATIONS_H

#include <domain/settings/Settings.h>

class DiveEquations {
public:

    /**
     * Execute the schreiner equation
     *
     * @param pBegin Initial compartment inert gas pressure (bar)
     * @param pGas Initial ambient pressure minus water vapor pressure (bar)
     * @param duration Time of exposure or interval in minutes.
     * @param gasRate Rate of descent/ascent in bar times the fraction of inert gas.
     * @return The end compartment inert gas pressure in bar.
     */
    static double
    schreinerEquation(double pBegin, double pGas, double durationInMinutes, double timeConstant, double gasRate);


    /**
     * Execute the instantaneous equation
     *
     * @param pBegin Initial compartment inert gas pressure (bar)
     * @param pGas Initial ambient pressure minus water vapor pressure (bar)
     * @param time Time interval (minutes)
     * @param halfTime Halftime of inhert gas (minutes)
     * @return End compartment inert gas pressure (bar)
     */
    static double instantaneousEquation(double pBegin, double pGas, double durationInMinutes, double timeConstant);

    /**
     * Calculates the approximate pressure of the fraction of gas for each breath taken.
     *
     * @param pressureInBars The pressure in bars.
     * @param fGas The fraction of the gas taken in.
     * @return The gas pressure in bars taken in with each breath (accounting for water vapour pressure in the lungs).
    */
    static double gasPressureBreathingInBars(double pressureInBars, double fGas);

    /**
     * Calculates the approximate pressure of the fraction of gas for each breath taken.
     *
     * @param depth The pressure in bars.
     * @param fGas The fraction of the gas taken in.
     * @return The gas pressure in bars taken in with each breath (NOT accounting for water vapour pressure in the lungs).
     */
    static double gasPressureInBars(double pressureInBars, double fGas);

    /**
     * Calculates the gas loading rate for the given pressure change in terms of bars inert gas.
     *
     * @param startPressureInBars The starting depth in bars.
     * @param endPressureInBars The end depth in bars.
     * @param duration The time that lapsed during the depth change in minutes.
     * @param fGas The fraction of gas to calculate for
     * @return The gas loading rate in bars times the fraction of inert gas.
     */
    static double
    gasRateInBarsPerMinute(double startPressureInBars, double endPressureInBars, double durationInMinutes, double fGas);

    /**
     * Calculates the absolute pressure (in bars) for 1 cubic meter of water for the given depth (meters).
     *
     * @param depth The depth in meters below the surface for 1 cubic meter volume of water.
     * @return The depth change in bars per minute.
     */
    static double depthInMetersToBars(double depthInMeters);

    /**
     * Calculates the depth (in meters) for the given atmosphere (bar).
     *
     * @param presureInBars The number of atmospheric pressure (in presureInBars) to convert.
     * @return The depth (in meters) for the given number of atmospheres.
     */
    static double barToDepthInMeters(double presureInBars);


    /**
    * Calculates the approximate pressure of the fraction of gas for each breath taken.
    *
    * @param depth The depth in meters.
    * @param fGas The fraction of the inert gas taken in.
    * @param fO2 The fraction of the O2 gas taken in.
    * @param setPoint The setPoint used.
    * @return The diluent gas pressure in bars taken in with each breath (accounting for water vapour pressure in the lungs).
    */
    static double gasCcrPressureBreathingInBars(double depthInMeters, double fGas, double fO2, double setPoint);

    static double
    ccrSchreinerEquation(double pBegin, double inertFraction, double o2Fraction, double halfTime, double duration,
                         double beginDepth, double endDepth, double setPoint);

};

#endif