#ifndef DIVEC_LOGBOOK_H
#define DIVEC_LOGBOOK_H

#include <filesystem/FileSystem.h>
#include <RTClib.h>
#include <domain/dive/Dive.h>

class LogBook final : public JsonSerializable {
public:

    void init(FileSystem *fileSystem);

    // logging dives
    Dive *loadDive(uint16_t diveNr);

    void saveDive(Dive *dive);

    void initTmpDiveLog();

    void addDiveStep(DiveStep *diveStep);


    // accessors
    uint16_t getNumberOfDives() const;

    void setNumberOfDives(uint16_t numberOfDives);

    double getMaximumDepth() const;

    void setMaximumDepth(double maximumDepth);

    uint32_t getTotalDiveTime() const;

    void setTotalDiveTime(uint32_t totalDiveTime);

    uint32_t getLastDiveDate() const;

    void setLastDiveDate(uint32_t lastDiveDate);


    // persistence

    JsonObject serializeObject(JsonObject &doc) final;

    void deserializeObject(JsonObject &doc) final;

    size_t getJsonSize() final;

private:
    uint16_t _numberOfDives = 0;
    double _maximumDepth = 0.0;
    uint32_t _totalDiveTime = 0;
    uint32_t _lastDiveDate = 0;

    FileSystem *_fileSystem;

};


#endif //DIVEC_LOGBOOK_H
