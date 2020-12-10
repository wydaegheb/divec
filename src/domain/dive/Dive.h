#ifndef DIVEC_DIVE_H
#define DIVEC_DIVE_H

#include <list>
#include <time/Time.h>
#include <domain/dive/DiveStep.h>

#define MAX_NR_OF_STEPS 240 // used for arduino json allocation

class Dive : public JsonSerializable {
public:
    Dive();

    ~Dive() override;

    DiveStep *update(uint32_t time, Gas *gas, double pressureInBar, double tempInCelsius);

    void clearEntries();

    void addStep(DiveStep *diveStep);

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

    std::list<DiveStep *> getSteps();

    JsonObject serializeObject(JsonObject &doc) override;

    void deserializeObject(JsonObject &doc) override;

    size_t getFileSize() override;

    void log();

    void log(Print *print);


private:
    bool _started;
    bool _surfaced;
    bool _ended; // not started doesn't necessarily mean it is ended. a dive can only be ended if it has been started before.

    uint32_t _startTime;
    uint32_t _lastTimeStamp;
    uint32_t _surfacedTimeInSeconds; // time when we surfaced after the dive (i.e. surface time as seconds since Jan 1 1970)

    double _avgDepthInMeter = 0.0;
    double _maxDepthInMeter = 0.0; // we should at least get in the water
    int8_t _minTemperatureInCelsius = 100; // diving in boiling water seems somewhat extreme
    int8_t _maxTemperatureInCelsius = 0; // water temp will (in practice) always be above 0C (otherwise it would be ice which makes it somewhat hard to dive)

    std::list<DiveStep *> _steps;

    void compressSteps();

};

#endif