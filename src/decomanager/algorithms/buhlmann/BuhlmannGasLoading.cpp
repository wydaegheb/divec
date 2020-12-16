#include "BuhlmannGasLoading.h"


BuhlmannGasLoading::BuhlmannGasLoading(BuhlmannTable buhlmannTable) {
    for (uint8_t i = 0; i < 16; i++) {
        _tissues.push_back(new BuhlmannTissue(buhlmannTable, i));
    }
    _lastPressureInBar = Settings::SURFACE_PRESSURE;
}

void BuhlmannGasLoading::resetTissues() {
    for (auto tissue : _tissues) {
        tissue->resetTissue();
    }
    _lastPressureInBar = Settings::SURFACE_PRESSURE;
}

void BuhlmannGasLoading::update(uint32_t beginTimeInSeconds, uint32_t endTimeInSeconds, double beginPressureInBar, double endPressureInBar, double gasN2Fraction, double gasHeFraction) {
    for (auto tissue : _tissues) {
        tissue->update(beginTimeInSeconds, endTimeInSeconds, beginPressureInBar, endPressureInBar, gasN2Fraction, gasHeFraction);
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

uint16_t BuhlmannGasLoading::getCeilingRoundedToDecoStepSize(double gradientFactor) {
    uint16_t ceiling = lround(getCeilingInMeters(gradientFactor));
    while (ceiling % Settings::DECO_STEP_SIZE != 0) {
        ceiling++;
    }

    return ceiling;
}


DecompressionPlan *BuhlmannGasLoading::getDecoPlan(GasManager *gasManager) {
    Serial.println(F(" - calculating deco plan"));
    auto *decoPlan = &DecompressionPlan::getInstance();
    decoPlan->init();

    double currentDepthInMeters = DiveEquations::barToDepthInMeters(_lastPressureInBar);
    if (currentDepthInMeters == 0.0) {
        return decoPlan;
    }

    // First ceiling
    double ceilingInMeter = getCeilingRoundedToDecoStepSize(Settings::GF_LOW);

    // Gradient factors
    //double distanceToSurfaceInMeters = ceilingInMeter;
    double distanceToSurfaceInMeters = ceilingInMeter;
    double gfChangePerMeter = (Settings::GF_HIGH - Settings::GF_LOW) / -distanceToSurfaceInMeters;

    startDecoCalculation(); // stores current state of tissues so we can revert to this state when our calculation is over

    // mount to first ceiling (possibly adds multiple steps if there are better gasses available while coming up)
    Gas *currentGas = addDecoDepthChange(currentDepthInMeters, ceilingInMeter, gasManager, decoPlan);

    uint16_t nextDecompressionDepth;
    uint32_t decoStopTimeInSeconds;
    double gradientFactor;
    while (ceilingInMeter > 0) {
        currentDepthInMeters = ceilingInMeter;
        nextDecompressionDepth = (ceilingInMeter - Settings::DECO_STEP_SIZE);
        if (nextDecompressionDepth < 0) {
            nextDecompressionDepth = 0;
        }

        decoStopTimeInSeconds = 0;
        gradientFactor = Settings::GF_HIGH + (gfChangePerMeter * nextDecompressionDepth);

        while (ceilingInMeter > nextDecompressionDepth && decoStopTimeInSeconds < 600000) {
            addDecoStop(currentDepthInMeters, Settings::MIN_STOP_TIME, currentGas, decoPlan);
            decoStopTimeInSeconds += Settings::MIN_STOP_TIME;
            ceilingInMeter = getCeilingInMeters(gradientFactor);
        }
        ceilingInMeter = nextDecompressionDepth;

        currentGas = addDecoDepthChange(currentDepthInMeters, nextDecompressionDepth, gasManager, decoPlan);
    }

    stopDecoCalculation(); // restores current state of tissues

    return decoPlan;
}

uint32_t BuhlmannGasLoading::getNdlInSeconds(GasManager *gasManager) {
    Serial.print(F(" - calculating ndl: "));

    double currentDepthInMeters = DiveEquations::barToDepthInMeters(_lastPressureInBar);
    if (currentDepthInMeters <= 0.0) {
        Serial.println(" 99 min");
        return MAX_NDL;
    }

    // First ceiling
    double ceilingInMeter = getCeilingInMeters(Settings::GF_LOW);

    if (ceilingInMeter > 0.0) { // in deco already
        Serial.println(" 0 min (already in deco)");
        return 0;
    }

    startDecoCalculation(); // stores current state of tissues so we can revert to this state when our calculation is over

    uint32_t ndlStopTimeInSeconds = 0;
    while (ceilingInMeter <= 0.0 && ndlStopTimeInSeconds < MAX_NDL) {
        // simulate adding gas loading on current depth and current gas for Settings::MIN_STOP_TIME
        for (auto _tissue : _tissues) {
            _tissue->addConstantDepthDiveStep(_lastPressureInBar, gasManager->getCurrentOcGas()->getN2(), gasManager->getCurrentOcGas()->getHe(), Settings::MIN_STOP_TIME);
        }
        ndlStopTimeInSeconds += Settings::MIN_STOP_TIME;
        ceilingInMeter = getCeilingInMeters(Settings::GF_LOW); // check ceiling again
    }

    stopDecoCalculation(); // restores current state of tissues

    Serial.print(ndlStopTimeInSeconds / 60.0);
    Serial.println(" min");

    return ndlStopTimeInSeconds;
}

void BuhlmannGasLoading::startDecoCalculation() {
    for (auto _tissue : _tissues) {
        _tissue->startSimulation();
    }
}

void BuhlmannGasLoading::stopDecoCalculation() {
    for (auto _tissue : _tissues) {
        _tissue->undoSimulation();
    }
}

void BuhlmannGasLoading::addDecoStop(uint16_t depthInMeter, uint32_t durationInSeconds, Gas *diveGas, DecompressionPlan *decompressionPlan) {
    decompressionPlan->addStop(diveGas, durationInSeconds, depthInMeter);
    // update tissues
    for (auto _tissue : _tissues) {
        _tissue->addConstantDepthDiveStep(DiveEquations::depthInMetersToBars(depthInMeter), diveGas->getN2(), diveGas->getHe(), durationInSeconds);
    }
}

Gas *BuhlmannGasLoading::addDecoDepthChange(uint16_t fromDepthInMeter, uint16_t toDepthInMeter, GasManager *gasManager, DecompressionPlan *decompressionPlan) {
    Gas *currentGas = gasManager->getBestOcGas(fromDepthInMeter);
    if (currentGas == nullptr) {
        Serial.print(F("Unable to find starting gas to decompress at depth: "));
        Serial.print(fromDepthInMeter);
        Serial.println(F(" - !!!!WARNING!!!! continuing with current gas but this is an illegal gas to use at this depth!!"));
        currentGas = gasManager->getCurrentOcGas();
    }

    // toDepthInMeter is the ceiling based on tissue loading
    while (fromDepthInMeter > toDepthInMeter) { // ascent till we reach this ceiling.
        // check the best gas for this depth
        Gas *bestGas = gasManager->getBestOcGas(fromDepthInMeter);

        if (bestGas != nullptr && (strcmp(bestGas->getName(), currentGas->getName()) != 0)) {
            Serial.println(F("We are not on the best possible gas!  Switching to this gas."));
            currentGas = bestGas;
        }

        // calculate ceiling. Normally ceiling is toDepth, unless there's a better gas to switch to on the way up.
        uint16_t ceilingInMeter = toDepthInMeter;
        for (int16_t nextDepth = fromDepthInMeter - 1; nextDepth >= ceilingInMeter; nextDepth--) {
            Gas *nextDecoGasName = gasManager->getBestOcGas(nextDepth);
            if (nextDecoGasName != nullptr && strcmp(nextDecoGasName->getName(), currentGas->getName()) != 0) {
                Serial.print(F("GAS SWITCH IN WAY UP! - "));
                Serial.print(F("Ceiling was: "));
                Serial.print(ceilingInMeter);
                Serial.print(F(" adapted to: "));
                Serial.println(nextDepth);
                ceilingInMeter = nextDepth; // Only carry us up to the point where we can use this better gas.
                break;
            }
        }

        // take us to the (new) ceiling at 10 mpm (the fastest ascent rate possible.)
        double durationInSeconds = ((fromDepthInMeter - ceilingInMeter) / 10.0) * 60.0; // TODO: on helium the max ascent rate is 9 m/minute. should take this into account

        // add to plan
        decompressionPlan->addDecoDepthChange(currentGas, durationInSeconds, fromDepthInMeter, ceilingInMeter);

        // update tissues
        for (auto _tissue : _tissues) {
            _tissue->addDepthChangingDiveStep(DiveEquations::depthInMetersToBars(fromDepthInMeter), DiveEquations::depthInMetersToBars(ceilingInMeter), currentGas->getN2(),
                                              currentGas->getHe(), durationInSeconds);
        }

        fromDepthInMeter = ceilingInMeter; // move up from-depth
    }
    Gas *bestGas = gasManager->getBestOcGas(fromDepthInMeter);
    if (bestGas != nullptr && (strcmp(bestGas->getName(), currentGas->getName()) != 0)) {
        Serial.println(F("Switching to better gas."));
        currentGas = bestGas;
    }
    return currentGas;
}

double BuhlmannGasLoading::getLastPressureInBar() const {
    return _lastPressureInBar;
}

void BuhlmannGasLoading::setLastPressureInBar(double lastPressureInBar) {
    _lastPressureInBar = lastPressureInBar;
}

const std::list<BuhlmannTissue *> &BuhlmannGasLoading::getTissues() const {
    return _tissues;
}

void BuhlmannGasLoading::setTissues(const std::list<BuhlmannTissue *> &tissues) {
    _tissues = tissues;
}


















