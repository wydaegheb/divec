#include "DepthSensor.h"

void DepthSensor::init(bool isMocked) {
    Serial.println(F("Initializing clock."));
    _isMocked = isMocked;
    if (isMocked){
        Serial.println(F(" - using mock depth sensor."));
        _currentPressure = 1.0;
    } else {
        Wire.begin();
        if (!_depthSensor.init()) {
            Serial.println(F("!!! SYSTEM ERROR !!!\n[Depth sensor failed]"));
            exit(0);
        } else {
            _depthSensor.setModel(DIVEC_MS5837::MS5837_02BA);
            //_depthSensor.setModel(DIVEC_MS5837::MS5837_30BA);
            Serial.println(F(" - depth sensor initialized"));
        }
    }

}

double DepthSensor::pressureInBar() {
    if (_isMocked){
        _currentPressure = _currentPressure + 0.1;
        return _currentPressure;
    } else {
        return _depthSensor.pressure() / 1000.0;
    }
}

double DepthSensor::tempInCelsius() {
    if (_isMocked){
        return 20.0;
    } else {
        return _depthSensor.temperature() / 100.0;
    }
}

void DepthSensor::read() { // use carefully! reading the depth sensor takes time
    if (!_isMocked){
        _depthSensor.read();
    }
    // doesn't do anything when mocked
}
