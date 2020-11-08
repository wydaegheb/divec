#include "Formatter.h"

char *Formatter::formatTime(char *timeStr, uint32_t seconds, bool removeLeadingZeroes) {
    return Formatter::formatTime(timeStr, TimeSpan(seconds), removeLeadingZeroes);
}

char *Formatter::formatTime(char *timeStr, const TimeSpan &timeSpan, bool removeLeadingZeroes) {
    char hrStr[3] = "";
    char minStr[3] = "";
    char secStr[3] = "";
    bool hasHours = timeSpan.hours() > 0;
    bool hasMinutes = timeSpan.minutes() > 0;
    if (hasHours || !removeLeadingZeroes) {
        utoa(timeSpan.hours(), hrStr, 10);
        strcat(timeStr, hrStr);
        strcat(timeStr, ":");
    }
    if (hasHours || hasMinutes || !removeLeadingZeroes) {
        utoa(timeSpan.minutes(), minStr, 10);
        strcat(timeStr, minStr);
        strcat(timeStr, ":");
    }
    if (removeLeadingZeroes && !hasHours && !hasMinutes) {
        strcat(timeStr, "0:");
    }
    if (timeSpan.seconds() < 10) {
        strcat(timeStr, "0");
    }
    utoa(timeSpan.seconds(), secStr, 10);
    strcat(timeStr, secStr);
    return timeStr;
}

char *Formatter::formatTimeInMinutes(char *timeStr, uint32_t seconds, bool roundMinutes) {
    char minStr[3] = "";
    char secStr[3] = "";
    TimeSpan timeSpan = TimeSpan(seconds);
    uint8_t min = timeSpan.minutes();
    min += timeSpan.hours() * 60;
    if (roundMinutes && timeSpan.seconds() > 30) {
        min++;
    }
    utoa(min, minStr, 10);
    strcat(timeStr, minStr);

    if (!roundMinutes) {
        strcat(timeStr, ":");
        if (timeSpan.seconds() < 10) {
            strcat(timeStr, "0");
        }
        utoa(timeSpan.seconds(), secStr, 10);
        strcat(timeStr, secStr);
    }

    return timeStr;
}

