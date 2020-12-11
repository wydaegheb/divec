#ifndef DIVEC_DIVESTEP_H
#define DIVEC_DIVESTEP_H

#include <Arduino.h>
#include <domain/dive/Gas.h>

class DiveStep final : public JsonSerializable {
public:
    DiveStep() = default;

    DiveStep(uint32_t endTime, Gas *gas, double pressureInBar, double temperature);

    uint32_t getEndTime();

    void setEndTime(uint32_t endTime);

    double getPressureInBar() const;

    void setPressureInBar(double pressureInBar);

    double getTemperatureInCelsius() const;

    void setTemperatureInCelsius(double temperatureInCelsius);

    char const *getGasName();

    void setGasName(Gas *gas);

    JsonObject serializeObject(JsonObject &doc) final;

    void deserializeObject(JsonObject &doc) final;

    size_t getJsonSize() final;

    void log();

    void log(Print *print);

private:
    uint32_t _endTime;
    double _pressureInBar;
    double _temperatureInCelsius;
    char const *_gasName;
};


#endif
