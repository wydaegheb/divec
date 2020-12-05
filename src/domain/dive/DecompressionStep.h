#ifndef DIVEC_DECOMPRESSIONSTEP_H
#define DIVEC_DECOMPRESSIONSTEP_H

#include <time/Time.h>
#include <display/utils/Formatter.h>
#include <domain/dive/Gas.h>

class DecompressionStep {
public:

    DecompressionStep(Gas *diveGas, uint16_t durationInSeconds, uint16_t depthInMeters);

    DecompressionStep(Gas *diveGas, uint16_t durationInSeconds, uint16_t startDepthInMeters, uint16_t endDepthInMeters);

    ~DecompressionStep();

    Gas *getDiveGas();

    void setDiveGas(Gas *diveGas);

    uint16_t getStartDepthInMeters() const;

    void setStartDepthInMeters(uint16_t startDepth);

    uint16_t getEndDepthInMeters() const;

    void setEndDepthInMeters(uint16_t endDepth);

    uint16_t getDurationInSeconds() const;

    void setDurationInSeconds(uint16_t durationInSeconds);

    bool isFlat() const;

    uint32_t log(uint32_t time);

    uint32_t log(Print *print, uint32_t time);


private:
    uint16_t _durationInSeconds;
    uint16_t _startDepthInMeters;
    uint16_t _endDepthInMeters;

    Gas *_diveGas;

};

#endif