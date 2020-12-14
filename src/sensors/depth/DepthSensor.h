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

    bool isMocked() const;

    void increaseMockDepth();

    void decreaseMockDepth();

private:
    DIVEC_MS5837 _depthSensor;
    bool _mocked;
    uint32_t _lastUpdatedInMillis;
    double _currentPressure;
    double _currentTemp;

};

#endif
