#ifndef DECOMPRESSION_PLAN_H
#define DECOMPRESSION_PLAN_H

#include <list>
#include <domain/dive/DecompressionStep.h>
#include <domain/dive/Dive.h>
#include <display/utils/Formatter.h>


class DecompressionPlan {
public:
    ~DecompressionPlan();

    void addStop(Gas *gas, uint32_t timeInSeconds, uint16_t depthInMeter);

    void addDecoDepthChange(Gas *gas, uint32_t timeInSeconds, uint16_t startDepth, uint16_t endDepth);

    std::list<DecompressionStep *> getStops();

    uint32_t getNdlInSeconds();

    uint32_t getTtsInSeconds();

    void log();
    void log(Dive *dive);

    void log(Print *print, Dive *dive);


private:
    std::list<DecompressionStep *> _stops;
};

#endif