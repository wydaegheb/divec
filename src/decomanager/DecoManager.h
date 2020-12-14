#ifndef DIVEC_DECOMANAGER_H
#define DIVEC_DECOMANAGER_H

#include <time/Time.h>
#include <gasmanager/GasManager.h>
#include <decomanager/algorithms/DiveAlgorithm.h>
#include <decomanager/algorithms/buhlmann/BuhlmannAlgorithm.h>
#include <filesystem/JsonSerializable.h>
#include <filesystem/FileSystem.h>
#include <domain/logbook/LogBook.h>

class DecoManager final : public JsonSerializable {
public:

    void init(FileSystem *fileSystem, uint32_t currentTimeInSeconds);

    void update(uint32_t currentTimeInSeconds, double currentPressureInBar, double tempInCelsius, bool wetContactActivated);

    DecompressionPlan *getDecoPlan();

    // reset all - CLEARS tissues!
    void reset(uint32_t currentTimeInSeconds);


    // dive
    Dive *getDive();

    Dive *loadDive(uint16_t diveNr);

    void startDive(uint32_t currentTimeInSeconds);

    void endDive();

    const LogBook *getLogBook() const;


    // algorithm
    void setCurrentAlgorithm(char const *namename);

    DiveAlgorithm *getCurrentAlgorithm();

    void addAlgorithm(DiveAlgorithm *diveAlgorithm);


    // gas
    GasManager *getGasManager();

    Gas *getCurrentGas();


    // state
    uint32_t getNdlInSeconds();

    uint32_t getPreviousUpdateTime() const;

    double getPreviousPressureInBar() const;


    // surface interval
    void addSurfaceInterval(uint32_t beginTime, uint32_t endTime);

    uint32_t getSurfaceIntervalInSeconds();


    // persistence
    JsonObject serializeObject(JsonObject &doc) final;

    void deserializeObject(JsonObject &doc) final;

    size_t getJsonSize() final;

private:
    FileSystem *_fileSystem;

    LogBook *_logBook;

    Dive *_dive;
    DiveAlgorithm *_currentAlgorithm;
    std::list<DiveAlgorithm *> _algorithms;
    GasManager *_gasManager;

    uint32_t _previousUpdateTimeInSeconds;
    double _previousPressureInBar;

};


#endif //DIVEC_DECOMANAGER_H
