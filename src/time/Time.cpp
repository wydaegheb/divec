#include "Time.h"

uint32_t  Time::_mockTime;
boolean  Time::_mocked;
RTC_DS3231 Time::_rtc;

uint32_t Time::getTime() {
    if (_mocked) {
        return _mockTime;
    }
    return _rtc.now().unixtime();
}

void Time::incMockTime() {
    Serial.print(F("Inc time: "));
    _mockTime += 60;
    Serial.println(_mockTime);

}

bool Time::init(bool isMocked) {
    Serial.println(F("Initializing clock."));
    _mocked = isMocked;
    if (_mocked) {
        Serial.println(F(" - using mock time."));
        _mockTime = 0;
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

    //_rtc.adjust(DateTime(__DATE__, __TIME__));

    Serial.print(F(" - clock initialized: "));
    Serial.println(DateTime(getTime()).timestamp());
    return true;
}


