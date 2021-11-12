#ifndef DIVEC_DIVE_ALGORITHM_H
#define DIVEC_DIVE_ALGORITHM_H

#include <domain/dive/Dive.h>
#include <domain/dive/DecompressionPlan.h>
#include <gasmanager/GasManager.h>

class DiveAlgorithm : public JsonSerializable {
public:

    virtual void update(uint32_t beginTimeInSeconds, uint32_t endTimeInSeconds, Gas *gas, double beginPressureInBar,
                        double endPressureInBar) = 0;

    virtual DecompressionPlan *getDecoPlan(GasManager *gasManager, uint32_t runtimeInSeconds) = 0;

    virtual uint32_t getNdlInSeconds(GasManager *gasManager) = 0;

    virtual void applySurfaceInterval(uint32_t duration) = 0;

    virtual void reset() = 0;
};


#endif