#include <iostream>
#include "BuhlmannGasLoading.h"


BuhlmannGasLoading::BuhlmannGasLoading(BuhlmannTable buhlmannTable) {
    for (uint8_t i = 0; i < 16; i++) {
        _tissues[i] = new BuhlmannTissue(buhlmannTable, i);
    }
    _lastPressureInBar = Settings::SURFACE_PRESSURE;
}

void BuhlmannGasLoading::resetTissues() {
    for (auto &_tissue : _tissues) {
        _tissue->resetTissue();
    }
    _lastPressureInBar = Settings::SURFACE_PRESSURE;
}

void BuhlmannGasLoading::applySurfaceInterval(uint32_t durationInSeconds) {
    update(0, durationInSeconds, Settings::SURFACE_PRESSURE, Settings::SURFACE_PRESSURE, GasManager::AIR.getN2frac(), GasManager::AIR.getHefrac());
}

void BuhlmannGasLoading::update(uint32_t beginTimeInSeconds, uint32_t endTimeInSeconds, double beginPressureInBar, double endPressureInBar, double gasN2Fraction, double gasHeFraction) {
    double durationInMinutes = (endTimeInSeconds - beginTimeInSeconds) / 60.0;
    for (auto &_tissue : _tissues) {
        _tissue->update(durationInMinutes, beginPressureInBar, endPressureInBar, gasN2Fraction, gasHeFraction);
    }
    _lastPressureInBar = endPressureInBar;
}


double BuhlmannGasLoading::getCeilingInMeters(double gradientFactor) {
    double ceiling = 0.0;
    for (auto tissue : _tissues) {
        double tissueCeiling = tissue->calculateCeilingInMeter(gradientFactor);
        if (tissueCeiling > ceiling) {
            ceiling = tissueCeiling;
        }
    }
    return ceiling;
}


DecompressionPlan *BuhlmannGasLoading::getDecoPlan(GasManager *gasManager, uint32_t runtimeInSeconds) {
    auto *decoPlan = &DecompressionPlan::getInstance();
    decoPlan->init();
    double currentDepthInMeters = DiveEquations::barToDepthInMeters(_lastPressureInBar);

    // stores current state of tissues so we can revert to this state when our calculation is over
    startDecoCalculation();

    // ASCENT TILL FIRST DECOSTOP
    // Ascent in DECO_STEP_SIZE (3m) intervals - note: these are no deco stops - just coming up until we hit the ceiling.
    // we cannot come up directly to the ceiling we just calculated as in very rare cases the ceiling comes further down during our ascent to this ceiling
    // due to further on-gassing in the way up. (EC Baker "Note: this situation is a possibility when ascending from extremely deep dives or due to an unusual gas mix selection.")
    double ceilingInMeter = getCeilingInMeters(Settings::GF_LOW);
    double nextDepthInDecoStepSize = floor((currentDepthInMeters / Settings::DECO_STEP_SIZE) - 0.5) * Settings::DECO_STEP_SIZE;
    Gas *currentGas = gasManager->getCurrentGas();
    while (nextDepthInDecoStepSize > ceilingInMeter) {
        runtimeInSeconds += addDecoDepthChange(currentDepthInMeters, nextDepthInDecoStepSize, currentGas, decoPlan);
        currentDepthInMeters = nextDepthInDecoStepSize;
        nextDepthInDecoStepSize = currentDepthInMeters - Settings::DECO_STEP_SIZE;
        ceilingInMeter = getCeilingInMeters(Settings::GF_LOW);
    }

    // START DECO STOPS
    // do stops every DECO_STEP_SIZE (3m) till we reach the surface
    double decoStopDepth = currentDepthInMeters;
    double nextDecoStopDepth = nextDepthInDecoStepSize;
    double gfChangePerMeter = (Settings::GF_HIGH - Settings::GF_LOW) / -decoStopDepth;// Calculate Gradient factor slope (note GF_LOW is applied starting at the first deco stop and not from the bottom)
    double gradientFactor;
    while (decoStopDepth > 0) {
        gradientFactor = Settings::GF_HIGH + (gfChangePerMeter * nextDecoStopDepth);

        while (ceilingInMeter > nextDecoStopDepth) {
            // The part "Settings::MIN_STOP_TIME - secondsAboveMinDecoStopTimeMutliple(runtimeInSeconds)" rounds the runtime to next decostepsize (this seems to be for "cosmetic purposes" but as ECBaker does it, it is needed to get the exact same results)
            // this means that the first stop is NOT a multiple of Settings::MIN_STOP_TIME (stop can be smaller than Settings::MIN_STOP_TIME). By rounding up when printing the result this is "hidden" (a stop of 1.5 min becomes 2 min) but
            // if you look closely you'll see that the runtime doesn't match the rounded decostops.
            runtimeInSeconds += addDecoStop(decoStopDepth, Settings::MIN_STOP_TIME - secondsAboveMinDecoStopTimeMultiple(runtimeInSeconds), currentGas, decoPlan);
            ceilingInMeter = getCeilingInMeters(gradientFactor); // recalculate the ceiling (once the ceiling becomes higher than the next decostop we ascend to the next decostop.)
        }
        runtimeInSeconds += addDecoDepthChange(decoStopDepth, nextDecoStopDepth, currentGas, decoPlan); // add ascent to next decostop

        // update stop and next stop
        decoStopDepth = nextDecoStopDepth;
        nextDecoStopDepth = nextDecoStopDepth - Settings::DECO_STEP_SIZE;

        // check if there is a better gas available -> if so use it
        currentGas = gasManager->getBestGas(decoStopDepth);
    }

    stopDecoCalculation(); // restores current state of tissues

    return decoPlan;
}

uint32_t BuhlmannGasLoading::getNdlInSeconds(GasManager *gasManager) {
    double currentDepthInMeters = DiveEquations::barToDepthInMeters(_lastPressureInBar);
    if (currentDepthInMeters <= 0.0) {
        return MAX_NDL;
    }

    // First ceiling
    double ceilingInMeter = getCeilingInMeters(Settings::GF_LOW);

    if (ceilingInMeter > 0.0) { // in deco already
        return 0;
    }

    startDecoCalculation(); // stores current state of tissues so we can revert to this state when our calculation is over

    uint32_t ndlStopTimeInSeconds = 0;
    while (ceilingInMeter <= 0.0 && ndlStopTimeInSeconds < MAX_NDL) {
        // simulate adding gas loading on current depth and current gas for Settings::MIN_STOP_TIME
        for (auto _tissue : _tissues) {
            _tissue->addConstantDepthDiveStep(_lastPressureInBar, gasManager->getCurrentGas()->getN2frac(),
                                              gasManager->getCurrentGas()->getHefrac(), Settings::MIN_STOP_TIME);
        }
        ndlStopTimeInSeconds += Settings::MIN_STOP_TIME;
        ceilingInMeter = getCeilingInMeters(Settings::GF_LOW); // check ceiling again
    }

    stopDecoCalculation(); // restores current state of tissues

    return ndlStopTimeInSeconds;
}

// private methods

uint16_t BuhlmannGasLoading::addDecoStop(double depthInMeter, uint32_t durationInSeconds, Gas *diveGas, DecompressionPlan *decompressionPlan) {
    decompressionPlan->addStop(diveGas, durationInSeconds, depthInMeter);
    // update tissues
    for (auto tissue : _tissues) {
        tissue->addConstantDepthDiveStep(DiveEquations::depthInMetersToBars(depthInMeter), diveGas->getN2frac(), diveGas->getHefrac(), durationInSeconds);
    }
    return durationInSeconds;
}

uint16_t BuhlmannGasLoading::addDecoDepthChange(double fromDepthInMeter, double toDepthInMeter, Gas *diveGas, DecompressionPlan *decompressionPlan) {

    // take us to the next step depth at the fastest ascent rate possible.
    double durationInSeconds = ((fromDepthInMeter - toDepthInMeter) / getAscentRateInMeterPerMinute(fromDepthInMeter, diveGas)) * 60.0;

    // add to plan
    decompressionPlan->addDecoDepthChange(diveGas, durationInSeconds, fromDepthInMeter, toDepthInMeter);

    // update tissues
    for (auto tissue: _tissues) {
        tissue->addDepthChangingDiveStep(DiveEquations::depthInMetersToBars(fromDepthInMeter), DiveEquations::depthInMetersToBars(toDepthInMeter), diveGas->getN2frac(), diveGas->getHefrac(), durationInSeconds);
    }
    return durationInSeconds;
}


void BuhlmannGasLoading::startDecoCalculation() {
    for (auto tissue : _tissues) {
        tissue->startDecoCalculation();
    }
}

void BuhlmannGasLoading::stopDecoCalculation() {
    for (auto tissue : _tissues) {
        tissue->stopDecoCalculation();
    }
}

double BuhlmannGasLoading::getAscentRateInMeterPerMinute(double depthInMeter, Gas *diveGas) {
    return 10; // TODO: improve slower on helium, slower in the last 3 meters, etc...
}

uint16_t BuhlmannGasLoading::secondsAboveMinDecoStopTimeMultiple(uint32_t time) {
    return time - (time / Settings::MIN_STOP_TIME) * Settings::MIN_STOP_TIME;;
}

BuhlmannTissue *const *BuhlmannGasLoading::getTissues() const {
    return _tissues;
}

double BuhlmannGasLoading::getLastPressureInBar() const {
    return _lastPressureInBar;
}

void BuhlmannGasLoading::setLastPressureInBar(double lastPressureInBar) {
    _lastPressureInBar = lastPressureInBar;
}




















