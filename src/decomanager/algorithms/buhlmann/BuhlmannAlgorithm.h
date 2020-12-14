#ifndef DIVEC_BUHLMANN_ALGORITHM_H
#define DIVEC_BUHLMANN_ALGORITHM_H

#include <decomanager/algorithms/DiveAlgorithm.h>
#include <decomanager/algorithms/buhlmann/BuhlmannGasLoading.h>


class BuhlmannAlgorithm final : public DiveAlgorithm {
public:
    BuhlmannAlgorithm(char const *name, BuhlmannTable buhlmannTable);

    ~BuhlmannAlgorithm();

    void reset() final;

    void update(uint32_t beginTimeInSeconds, uint32_t endTimeInSeconds, GasManager *gasManager, double beginPressureInBar, double endPressureInBar) final;

    DecompressionPlan *getDecoPlan(GasManager *gasManager) final;

    uint32_t getNdlInSeconds(GasManager *gasManager) final;

    char const *getName() final;

    JsonObject serializeObject(JsonObject &doc) final;

    void deserializeObject(JsonObject &doc) final;

    size_t getJsonSize() final;

private:
    BuhlmannGasLoading *_buhlmannGasLoading;
    char const *_name;
};

#endif