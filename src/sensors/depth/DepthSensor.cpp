#include "DepthSensor.h"

void DepthSensor::init(bool isMocked) {
    Serial.println(F("Initializing depth sensor."));
    _mocked = isMocked;
    if (isMocked) {
        Serial.println(F(" - using mock depth sensor."));
        _currentPressure = 1000.0;
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
    _lastUpdatedInMillis = millis();
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
    // reading the depth sensor takes time - this way we avoid more than 2 readings every second - readings of the depth sensor should happen as few as possible
    if ((millis() - _lastUpdatedInMillis) > 500) {
        Serial.println(F(" - reading depth sensor."));
        if (!_mocked) {
            _depthSensor.read();
            //Check for sensor error - difference has to be less than 20 meters (otherwise keep previous pressure) -> copied from DiveIno.ino)
            if (abs(_currentPressure - _depthSensor.pressure()) < 20000.0) {
                _currentPressure = _depthSensor.pressure();
                Serial.print(F(" - reading depth sensor: currentPressure:"));
                Serial.println(_currentPressure);
            }

            //Check for sensor error - difference has to be less than 20 degrees celcius (otherwise keep previous temp)
            if (abs(_currentTemp - _depthSensor.temperature()) < 2000.0) {
                _currentTemp = _depthSensor.temperature();
                Serial.print(F(" - reading depth sensor: currentPressure:"));
                Serial.println(_currentTemp);
            }
        }
        _lastUpdatedInMillis = millis();
    } else {
        Serial.println(F(" - skipped reading depth sensor."));
    }
}

bool DepthSensor::isMocked() const {
    return _mocked;
}

void DepthSensor::increaseMockDepth() {

    _currentPressure += 100.0;
}

void DepthSensor::decreaseMockDepth() {
    if (_currentPressure > 99.0) {
        _currentPressure -= 100.0;
    }
}

