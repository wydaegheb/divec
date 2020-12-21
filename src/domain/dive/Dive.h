#ifndef DIVEC_DIVE_H
#define DIVEC_DIVE_H

#include <list>
#include <time/Time.h>
#include "Gas.h"

class Dive final : public JsonSerializable {
public:
    static Dive &getInstance();

    Dive(Dive const &) = delete;

    void operator=(Dive const &) = delete;

    void init();

    void *update(uint32_t time, Gas *gas, double pressureInBar, double tempInCelsius);

    void start(uint32_t startTime);

    void end();

    bool isStarted() const;

    bool isSurfaced() const;

    bool isInProgress() const;

    bool isEnded() const;

    double getCurrentDepthInMeters();

    uint32_t getDiveTimeInSeconds() const;

    uint32_t getLastTimeStamp() const;

    uint32_t getStartTime() const;

    uint16_t getAvgDepthInMeter() const;

    uint16_t getMaxDepthInMeter() const;

    int8_t getMinTemperatureInCelsius() const;

    int8_t getMaxTemperatureInCelsius() const;

    JsonObject serializeObject(JsonObject &doc) final;

    void deserializeObject(JsonObject &doc) final;

    size_t getJsonSize() final;

    void log();

    void log(Print *print);


private:
    bool _started;
    bool _surfaced;
    bool _ended; // not started doesn't necessarily mean it is ended. a dive can only be ended if it has been started before.

    uint32_t _startTime;
    uint32_t _lastTimeStamp;
    uint32_t _surfacedTimeInSeconds; // time when we surfaced after the dive (i.e. surface time as seconds since Jan 1 1970)

    double _currentDepthInMeter = 0.0;
    double _avgDepthInMeter = 0.0;
    double _maxDepthInMeter = 0.0; // we should at least get in the water
    int8_t _minTemperatureInCelsius = 100; // diving in boiling water seems somewhat extreme
    int8_t _maxTemperatureInCelsius = 0; // water temp will (in practice) always be above 0C (otherwise it would be ice which makes it somewhat hard to dive)

    int8_t _nrOfSteps = 0; // only needed to calculate average depth

    // NOTE: No need to keep a list of dive steps!
    // every step is logged to the sd card immediately and we don't need the individual steps for deco calculation

    Dive();

};

#endif