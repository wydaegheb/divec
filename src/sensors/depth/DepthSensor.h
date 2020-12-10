#ifndef DIVEC_DEPTHSENSOR_H
#define DIVEC_DEPTHSENSOR_H

#include <domain/settings/Settings.h>
#include <sensors/depth/DIVEC_MS5837.h>
#include <time/Time.h>

class DepthSensor {
public:
    void init(bool isMocked);

    void read();

    double tempInCelsius();

    double pressureInBar();

private:
    DIVEC_MS5837 _depthSensor;
    bool _isMocked;
    uint32_t _lastUpdated;
    double _currentPressure;
    double _currentTemp;

};

#endif
