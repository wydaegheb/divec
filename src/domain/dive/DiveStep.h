#ifndef DIVEC_DIVESTEP_H
#define DIVEC_DIVESTEP_H

#include "Arduino.h"
#include "Gas.h"

class DiveStep {
public:
    DiveStep(DateTime endTime, Gas *gas, double pressureInBar, double temperature);

    DateTime getEndTime();

    void setEndTime(DateTime endTime);

    double getPressureInBar();

    void setPressureInBar(double pressureInBar);

    double getTemperatureInCelsius();

    void setTemperatureInCelsius(double temperatureInCelsius);

    char const* getGasName();

    void setGasName(Gas *gas);

    void log();

    void log(Print *print);

private:
    DateTime _endTime;
    double _pressureInBar;
    double _temperatureInCelsius;
    char const* _gasName;
};


#endif
