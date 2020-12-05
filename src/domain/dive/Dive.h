#ifndef DIVEC_DIVE_H
#define DIVEC_DIVE_H

#include <list>
#include <time/Time.h>
#include <domain/dive/DiveStep.h>
#include <domain/logbook/LogBook.h>

class Dive : public JsonSerializable {
public:
    ~Dive() override;

    void update(const DateTime &time, Gas *gas, double pressureInBar, double tempInCelsius);

    void init(FileSystem *fileSystem);

    void clearEntries();

    void addStep(DiveStep *diveStep);

    void start(const DateTime &startTime);

    void end();

    bool isStarted() const;

    bool isSurfaced() const;

    bool isInProgress() const;

    bool isEnded() const;

    uint16_t getCurrentDepthInMeters();

    uint32_t getDiveTimeInSeconds() const;

    const DateTime &getLastTimeStamp() const;

    const DateTime &getStartTime() const;

    uint16_t getAvgDepthInMeter() const;

    uint16_t getMaxDepthInMeter() const;

    int8_t getMinTemperatureInCelsius() const;

    int8_t getMaxTemperatureInCelsius() const;

    std::list<DiveStep *> getSteps();

    size_t serialize(File *file) override;

    DeserializationError deserialize(File *file) override;

    size_t getFileSize() override;

    void log();

    void log(Print *print);


private:
    FileSystem *_fileSystem;
    LogBook *_logBook;

    bool _started;
    bool _surfaced;
    bool _ended; // not started doesn't necessarily mean it is ended. a dive can only be ended if it has been started before.

    DateTime _startTime;
    DateTime _lastTimeStamp;
    uint32_t _surfacedTimeInSeconds; // time when we surfaced after the dive (i.e. surface time as seconds since Jan 1 1970)

    uint16_t _avgDepthInMeter = 0;
    uint16_t _maxDepthInMeter = 0; // we should at least get in the water
    int8_t _minTemperatureInCelsius = 100; // diving in boiling water seems somewhat extreme
    int8_t _maxTemperatureInCelsius = 0; // water temp will always be above 0 or it would be ice which makes it somewhat hard to dive

    std::list<DiveStep *> _steps;

};

#endif