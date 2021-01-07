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

void Time::incOneMinute() {
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

void Time::setTime(uint8_t hours, uint8_t minutes, uint8_t seconds) {
    if (!_mocked) {
        DateTime now = _rtc.now();
        _rtc.adjust(DateTime(now.year(), now.month(), now.day(), hours, minutes, seconds));
    } else {
        DateTime now = DateTime(_mockTime);
        _mockTime = DateTime(now.year(), now.month(), now.day(), hours, minutes, seconds).unixtime();
    }
}

void Time::setDate(uint16_t year, uint8_t month, uint8_t day) {
    if (!_mocked) {
        DateTime now = _rtc.now();
        _rtc.adjust(DateTime(year, month, day, now.hour(), now.minute(), now.second()));
    } else {
        DateTime now = DateTime(_mockTime);
        _mockTime = DateTime(year, month, day, now.hour(), now.minute(), now.second()).unixtime();
    }
}

void Time::setDateTime(DateTime dateTime) {
    Serial.print(F("Setting:"));
    Serial.print(dateTime.day());
    Serial.print(F("/"));
    Serial.print(dateTime.month());
    Serial.print(F("/"));
    Serial.print(dateTime.year());
    Serial.print(F("  "));
    Serial.print(dateTime.hour());
    Serial.print(F(":"));
    Serial.print(dateTime.minute());
    Serial.print(F(":"));
    Serial.println(dateTime.second());
    if (!_mocked) {
        _rtc.adjust(dateTime);
    } else {
        _mockTime = dateTime.unixtime();
    }
}


