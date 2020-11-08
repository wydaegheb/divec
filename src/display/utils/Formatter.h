#ifndef DIVEC_FORMATTER_H
#define DIVEC_FORMATTER_H

#include <RTClib.h>

class Formatter {
public:
    static char *formatTime(char *timeStr, uint32_t seconds, bool removeLeadingZeroes);

    static char *formatTime(char *timeStr, const TimeSpan& timeSpan, bool removeLeadingZeroes);

    static char *formatTimeInMinutes(char *timeStr, uint32_t seconds, bool roundMinutes);
};


#endif //DIVEC_FORMATTER_H
