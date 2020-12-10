#ifndef DIVEC_TIME_H
#define DIVEC_TIME_H

#include <Arduino.h>
#include <RTClib.h>

class Time {
public:
    static bool init(bool isMocked);

    // in seconds
    static uint32_t getTime();

private:
    static boolean _mocked;
    static uint32_t _lastUpdateInMillis;

    static RTC_DS3231 _rtc;

};


#endif //DIVEC_TIME_H
