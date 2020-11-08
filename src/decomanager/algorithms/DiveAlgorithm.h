#ifndef DIVEC_DIVE_ALGORITHM_H
#define DIVEC_DIVE_ALGORITHM_H

#include <domain/dive/Dive.h>
#include <domain/dive/DecompressionPlan.h>
#include <gasmanager/GasManager.h>

class DiveAlgorithm {
public:
    virtual char const* getName() = 0;

    virtual void update(uint32_t beginTimeInSeconds, uint32_t endTimeInSeconds, GasManager *gasManager, double beginPressureInBar, double endPressureInBar) = 0;

    virtual DecompressionPlan *getDecoPlan(GasManager *gasManager) = 0;

    virtual uint32_t getNdlInSeconds() = 0;

    virtual JsonObject serialize(JsonObject &doc) = 0;

    virtual void deserialize(JsonObject &doc) = 0;

    virtual size_t getObjectSize() = 0;
};


#endif