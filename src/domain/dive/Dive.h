#ifndef DIVEC_DIVE_H
#define DIVEC_DIVE_H

#include <list>
#include <time/Time.h>
#include "DiveStep.h"

class Dive {
public:
    virtual ~Dive();

    void update(const DateTime& time, Gas *gas, double pressureInBar, double tempInCelsius);

    void init();

    void clearEntries();

    void start(const DateTime& startTime);

    bool isStarted() const;

    bool isSurfaced() const;

    bool isInProgress() const;

    bool isEnded() const;

    uint16_t getCurrentDepthInMeters();

    uint32_t getDiveTimeInSeconds() const;

    const DateTime &getLastTimeStamp() const;

    const DateTime &getStartTime() const;

    std::list<DiveStep *> getSteps();

    void log();

    void log(Print *print);


private:
    bool _started;
    bool _surfaced;
    bool _ended;
    DateTime _startTime;
    DateTime _lastTimeStamp;
    uint32_t _surfacedTimeInSeconds; // time in seconds since we surfaced after the dive
    std::list<DiveStep *> _steps;
};

#endif