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

    void addStop(Gas *gas, uint32_t timeInSeconds, uint16_t depthInMeter);

    void addDecoDepthChange(Gas *gas, uint32_t timeInSeconds, uint16_t startDepth, uint16_t endDepth);

    DecompressionStep *getFirstStop();

    std::list<DecompressionStep *> getStops();

    uint32_t getTtsInSeconds();

    void log();

    void log(Dive *dive);

    void log(Print *print, Dive *dive);


private:
    std::list<DecompressionStep *> _stops;

    DecompressionPlan();

};

#endif