#ifndef DECOMPRESSION_PLAN_H
#define DECOMPRESSION_PLAN_H

#include <list>
#include <domain/dive/DecompressionStep.h>
#include <domain/dive/Dive.h>
#include <display/utils/Formatter.h>


class DecompressionPlan {
public:
    static DecompressionPlan &getInstance();

    DecompressionPlan(DecompressionPlan const &) = delete;

    void operator=(DecompressionPlan const &) = delete;

    void init();

    void addStop(Gas *gas, uint32_t timeInSeconds, double depthInMeter);

    void addDecoDepthChange(Gas *gas, uint32_t timeInSeconds, double startDepth, double endDepth);

    uint32_t getTtsInSeconds();

    DecompressionStep *getFirstStop();

    void log(uint32_t planTime);

    void log(Print *print, uint32_t planTime);


private:
    DecompressionStep *_stops[150]; // max 150 deco stops (stop every 3 m -> max depth = 450 m)

    DecompressionPlan();

    uint8_t _nrOfStops;

};

#endif