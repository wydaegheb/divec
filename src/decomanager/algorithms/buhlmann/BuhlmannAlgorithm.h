#ifndef DIVEC_BUHLMANN_ALGORITHM_H
#define DIVEC_BUHLMANN_ALGORITHM_H

#include <decomanager/algorithms/DiveAlgorithm.h>
#include <decomanager/algorithms/buhlmann/BuhlmannGasLoading.h>


class BuhlmannAlgorithm final : public DiveAlgorithm {
public:
    BuhlmannAlgorithm(BuhlmannTable buhlmannTable);

    ~BuhlmannAlgorithm();

    void reset() final;

    void applySurfaceInterval(uint32_t duration) final;

    void update(uint32_t beginTimeInSeconds, uint32_t endTimeInSeconds, Gas *gas, double beginPressureInBar,
                double endPressureInBar) final;

    DecompressionPlan *getDecoPlan(GasManager *gasManager, uint32_t runtimeInSeconds) final;

    uint32_t getNdlInSeconds(GasManager *gasManager) final;

    JsonObject serializeObject(JsonObject &doc) final;

    void deserializeObject(JsonObject &doc) final;

    size_t getJsonSize() final;

private:
    BuhlmannGasLoading *_buhlmannGasLoading;
};

#endif