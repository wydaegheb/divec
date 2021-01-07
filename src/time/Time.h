#ifndef DIVEC_TIME_H
#define DIVEC_TIME_H

#include <Arduino.h>
#include <RTClib.h>

class Time {
public:
    static bool init(bool isMocked);

    // in seconds (unix time)
    static uint32_t getTime();

    static void incOneMinute();

    static void setDate(uint16_t year, uint8_t month, uint8_t day);

    static void setTime(uint8_t hours, uint8_t minutes, uint8_t seconds);

    static void setDateTime(DateTime dateTime);

private:
    static boolean _mocked;

    // in seconds (unix time)
    static uint32_t _mockTime;

    static RTC_DS3231 _rtc;

};


#endif //DIVEC_TIME_H
