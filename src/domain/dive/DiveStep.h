#ifndef DIVEC_DIVESTEP_H
#define DIVEC_DIVESTEP_H

#include <Arduino.h>
#include <domain/dive/Gas.h>

class DiveStep : public JsonSerializable {
public:
    DiveStep() = default;

    DiveStep(const DateTime &endTime, Gas *gas, double pressureInBar, double temperature);

    ~DiveStep() override = default;

    DateTime getEndTime();

    void setEndTime(const DateTime &endTime);

    double getPressureInBar() const;

    void setPressureInBar(double pressureInBar);

    double getTemperatureInCelsius() const;

    void setTemperatureInCelsius(double temperatureInCelsius);

    char const *getGasName();

    void setGasName(Gas *gas);

    JsonObject serializeObject(JsonObject &doc);

    size_t serialize(File *file) override;


    void deserializeObject(JsonObject &doc);

    DeserializationError deserialize(File *file) override;

    size_t getFileSize() override;

    void log();

    void log(Print *print);

private:
    DateTime _endTime;
    double _pressureInBar;
    double _temperatureInCelsius;
    char const *_gasName;
};


#endif
