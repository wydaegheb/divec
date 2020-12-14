#ifndef DIVEC_BUHLMANN_GAS_LOADING_H
#define DIVEC_BUHLMANN_GAS_LOADING_H

#include <domain/dive/DecompressionPlan.h>
#include <gasmanager/GasManager.h>
#include <decomanager/algorithms/buhlmann/BuhlmannTable.h>
#include <decomanager/algorithms/buhlmann/BuhlmannTissue.h>

#define MAX_NDL 99*60

class BuhlmannGasLoading {
public:

    explicit BuhlmannGasLoading(BuhlmannTable buhlmannTable);

    void resetTissues();

    void update(uint32_t beginTimeInSeconds, uint32_t endTimeInSeconds, double beginPressureInBar, double endPressureInBar, double gasN2Fraction, double gasHeFraction);

    double getCeilingInMeters(double gradientFactor);

    uint16_t getCeilingRoundedToDecoStepSize(double gradientFactor);

    DecompressionPlan *getDecoPlan(GasManager *gasManager);

    uint32_t getNdlInSeconds(GasManager *gasManager);

    double getLastPressureInBar() const;

    void setLastPressureInBar(double lastPressureInBar);

    const std::list<BuhlmannTissue *> &getTissues() const;

    void setTissues(const std::list<BuhlmannTissue *> &tissues);


private:
    void addDecoStop(uint16_t depthInMeter, uint32_t durationInSeconds, Gas *diveGas, DecompressionPlan *decompressionPlan) ;

    Gas *addDecoDepthChange(uint16_t fromDepthInMeter, uint16_t toDepthInMeter, GasManager *gasManager, DecompressionPlan *decompressionPlan);

    void startDecoCalculation();

    void stopDecoCalculation();

    double _lastPressureInBar;
    std::list<BuhlmannTissue *> _tissues;

};

#endif