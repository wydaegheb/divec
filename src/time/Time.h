#ifndef DIVEC_TIME_H
#define DIVEC_TIME_H

#include "Arduino.h"
#include "RTClib.h"

class Time {
public:
    static bool init(bool isMocked);

    static DateTime getTime();
    static void setTime();

private:
    static TimeSpan ONE_SECOND;
    static boolean _mocked;
    static uint32_t _lastMillis;
    static DateTime _mockedDateTime;

    static RTC_DS3231 _rtc;

};


#endif //DIVEC_TIME_H
