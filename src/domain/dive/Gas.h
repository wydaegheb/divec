#ifndef DIVEC_GAS_H
#define DIVEC_GAS_H

#include <decomanager/algorithms/equations/DiveEquations.h>

enum GasType : uint8_t {
    GAS_TYPE_OC = 0,  // also used for bailout
    GAS_TYPE_CC = 1
};

class Gas final : public JsonSerializable {
public:
    Gas() = default;

    explicit Gas(uint8_t o2, GasType gasType = GAS_TYPE_OC);

    Gas(uint8_t o2, uint8_t he, GasType gasType = GAS_TYPE_OC);

    Gas(uint8_t o2, uint8_t he, bool active, GasType gasType = GAS_TYPE_OC);

    GasType getGasType() const;

    void setGasType(GasType gasType);

    bool isActive() const;

    void setActive(bool active);

    char const *getName();

    double getO2frac() const;

    uint8_t getO2() const;

    void setO2(uint8_t o2);

    double getN2frac() const;

    uint8_t getN2() const;

    void setN2(uint8_t n2);

    double getHefrac() const;

    uint8_t getHe() const;

    void setHe(uint8_t he);

    bool isUsable(double pressureInBars) const;

    uint16_t calcMODInMeters() const;

    uint16_t calcENDInMeters(double pressureInBars) const;

    JsonObject serializeObject(JsonObject &doc) final;

    void deserializeObject(JsonObject &doc) final;

    size_t getJsonSize() final;

private:
    GasType _gasType;

    bool _active;

    // stored as integers (e.g. 20% o2 -> _o2 = 20). Total must be 100.
    uint8_t _o2;
    uint8_t _n2;
    uint8_t _he;

    char _name[10] = {0};

};

#endif