#ifndef DIVEC_BUHLMANN_ALGORITHM_H
#define DIVEC_BUHLMANN_ALGORITHM_H

#include <decomanager/algorithms/DiveAlgorithm.h>
#include "BuhlmannTable.h"
#include "BuhlmannGasLoading.h"


class BuhlmannAlgorithm : public DiveAlgorithm {
public:
    BuhlmannAlgorithm(char const* name, BuhlmannTable buhlmannTable);

    ~BuhlmannAlgorithm();

    void update(uint32_t beginTimeInSeconds, uint32_t endTimeInSeconds, GasManager *gasManager, double beginPressureInBar, double endPressureInBar) override;

    DecompressionPlan *getDecoPlan(GasManager *gasManager) override;

    uint32_t getNdlInSeconds() override;

    char const* getName() override;

    JsonObject serialize(JsonObject &doc) override;

    void deserialize(JsonObject &doc) override;

    size_t getObjectSize() override;

private:
    BuhlmannGasLoading *_buhlmannGasLoading;
    char const* _name;
};

#endif