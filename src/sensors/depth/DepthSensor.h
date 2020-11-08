#ifndef DIVEC_DEPTHSENSOR_H
#define DIVEC_DEPTHSENSOR_H

#include "domain/settings/Settings.h"
#include "DIVEC_MS5837.h"

class DepthSensor {
public:
    void init(bool isMocked);

    void read();

    double tempInCelsius();

    double pressureInBar();

private:
    DIVEC_MS5837 _depthSensor;
    bool _isMocked;
    double _currentPressure;

};

#endif
