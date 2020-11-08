#include "Time.h"

boolean  Time::_mocked;
uint32_t Time::_lastMillis;
DateTime Time::_mockedDateTime;
RTC_DS3231 Time::_rtc;

TimeSpan Time::ONE_SECOND = TimeSpan(1);

DateTime Time::getTime() {
    if (_mocked) {
        if ((millis() - _lastMillis) > 1000){
            _mockedDateTime = _mockedDateTime + Time::ONE_SECOND;
            _lastMillis = millis();
        }
        return _mockedDateTime;
    }
    return _rtc.now();
}

bool Time::init(bool isMocked) {
    Serial.println(F("Initializing clock."));
    _mocked = isMocked;
    if (_mocked) {
        Serial.println(F(" - using mock time."));
        _mockedDateTime = DateTime();
        _lastMillis = millis();
        return true;
    }

    Serial.println(F(" - using real clock."));

    if (!_rtc.begin()) {
        Serial.println(F(" - ERROR: Couldn't find Time Module!"));
        return false;
    }
    Serial.println(F(" - time module started."));

    if (_rtc.lostPower()) {
        Serial.println(F("WARNING: Real Time Clock lost power. Reset time to 1 jan 2020!"));
        _rtc.adjust(DateTime(2020, 1, 1, 12, 0, 0));
    }
    Serial.print(F(" - clock initialized: "));
    Serial.println(getTime().timestamp());
    return true;
}
