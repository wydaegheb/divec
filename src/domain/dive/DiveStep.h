#ifndef DIVEC_DIVESTEP_H
#define DIVEC_DIVESTEP_H

#include <Arduino.h>
#include <domain/dive/Gas.h>

class DiveStep : public JsonSerializable {
public:
    DiveStep() = default;

    DiveStep(uint32_t endTime, Gas *gas, double pressureInBar, double temperature);

    ~DiveStep() override = default;

    uint32_t getEndTime();

    void setEndTime(uint32_t endTime);

    double getPressureInBar() const;

    void setPressureInBar(double pressureInBar);

    double getTemperatureInCelsius() const;

    void setTemperatureInCelsius(double temperatureInCelsius);

    char const *getGasName();

    void setGasName(Gas *gas);

    JsonObject serializeObject(JsonObject &doc) override;

    void deserializeObject(JsonObject &doc) override;

    size_t getFileSize() override;

    void log();

    void log(Print *print);

private:
    uint32_t _endTime;
    double _pressureInBar;
    double _temperatureInCelsius;
    char const *_gasName;
};


#endif
