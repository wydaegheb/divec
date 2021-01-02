#ifndef DIVEC_TIME_H
#define DIVEC_TIME_H

#include <Arduino.h>
#include <RTClib.h>

// define date year range for editing
#define MIN_DATE_YEAR 2020
#define MAX_DATE_YEAR 2050

class Time {
public:
    static bool init(bool isMocked);

    // in seconds
    static uint32_t getTime();

    // in seconds
    static void incMockTime();

    static void setDate(uint16_t year, uint8_t month, uint8_t day);

    static void setTime(uint8_t hours, uint8_t minutes, uint8_t seconds);

private:
    static boolean _mocked;

    static uint32_t _mockTime;

    static RTC_DS3231 _rtc;

};


#endif //DIVEC_TIME_H
