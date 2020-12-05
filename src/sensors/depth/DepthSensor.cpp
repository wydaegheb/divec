#include "DepthSensor.h"

void DepthSensor::init(bool isMocked) {
    Serial.println(F("Initializing clock."));
    _isMocked = isMocked;
    if (isMocked) {
        Serial.println(F(" - using mock depth sensor."));
        _currentPressure = 0.0;
        _currentTemp = 2000.0;
    } else {
        Wire.begin();
        if (!_depthSensor.init()) {
            Serial.println(F("!!! SYSTEM ERROR !!!\n[Depth sensor failed]"));
            exit(0);
        } else {
            //_depthSensor.setModel(DIVEC_MS5837::MS5837_02BA);
            _depthSensor.setModel(DIVEC_MS5837::MS5837_30BA);
            Serial.println(F(" - depth sensor initialized"));
        }

        // do initial reading
        _depthSensor.read();
        _currentPressure = _depthSensor.pressure();
        _currentTemp = _depthSensor.temperature();
    }

}

double DepthSensor::pressureInBar() {
    read();
    return _currentPressure / 1000.0;
}

double DepthSensor::tempInCelsius() {
    read();
    return _currentTemp / 100.0;
}

void DepthSensor::read() {
    DateTime currentTime = Time::getTime();
    if ((currentTime.secondstime() - _lastUpdated.secondstime()) >
        500) { // reading the depth sensor takes time - this way we avoid more than 2 readings very second - ideally application readings of the depthsensor should happen as few as possible
        if (_isMocked) {
            _currentPressure = _currentPressure + 100.0;
            _currentTemp = 2000.0;
        } else {
            _depthSensor.read();
            //Check for sensor error - difference has to be less than 20 meters (otherwise keep previous pressure) -> copied from DiveIno.ino)
            if (abs(_currentPressure - _depthSensor.pressure()) < 2000) {
                _currentPressure = _depthSensor.pressure();
            }

            //Check for sensor error - difference has to be less than 20 degrees celcius (otherwise keep previous temp)
            if (abs(_currentTemp - _depthSensor.temperature()) < 2000) {
                _currentTemp = _depthSensor.temperature();
            }
        }
        _lastUpdated = Time::getTime();
    }
}
