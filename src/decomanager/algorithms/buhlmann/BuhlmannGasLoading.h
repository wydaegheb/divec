#ifndef DIVEC_BUHLMANN_GAS_LOADING_H
#define DIVEC_BUHLMANN_GAS_LOADING_H

#include <cmath>

#include <domain/dive/DecompressionPlan.h>
#include <gasmanager/GasManager.h>
#include <decomanager/algorithms/buhlmann/BuhlmannTable.h>
#include <decomanager/algorithms/buhlmann/BuhlmannTissue.h>

#define MAX_NDL 99*60

class BuhlmannGasLoading {
public:

    explicit BuhlmannGasLoading(BuhlmannTable buhlmannTable);

    void resetTissues();

    void applySurfaceInterval(uint32_t duration);

    void update(uint32_t beginTimeInSeconds, uint32_t endTimeInSeconds, double beginPressureInBar, double endPressureInBar, double gasN2Fraction, double gasHeFraction);

    double getCeilingInMeters(double gradientFactor);

    DecompressionPlan *getDecoPlan(GasManager *gasManager, uint32_t runtimeInSeconds);

    uint32_t getNdlInSeconds(GasManager *gasManager);

    BuhlmannTissue *const *getTissues() const;

    double getLastPressureInBar() const;

    void setLastPressureInBar(double lastPressureInBar);

private:
    BuhlmannTissue *_tissues[16];
    double _lastPressureInBar;

    uint16_t
    addDecoStop(double depthInMeter, uint32_t durationInSeconds, Gas *diveGas, DecompressionPlan *decompressionPlan);

    uint16_t addDecoDepthChange(double fromDepthInMeter, double toDepthInMeter, Gas *diveGas, DecompressionPlan *decompressionPlan);

    double getAscentRateInMeterPerMinute(double depthInMeter, Gas *diveGas);

    void startDecoCalculation();

    void stopDecoCalculation();

    uint16_t secondsAboveMinDecoStopTimeMultiple(uint32_t time);

};

#endif