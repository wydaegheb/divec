#ifndef DIVEC_DEPTHSENSOR_H
#define DIVEC_DEPTHSENSOR_H

#include <domain/settings/Settings.h>
#include <sensors/depth/DIVEC_MS5837.h>
#include <time/Time.h>

class DepthSensor {
public:
    DepthSensor() = default;

    bool init(bool isMocked); // this code would normally go into the constructor but then Serial.begin() is not yet called -> program locks up when doing Serial.print in constructor

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
