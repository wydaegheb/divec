#ifndef DIVEC_DECOMANAGER_H
#define DIVEC_DECOMANAGER_H

#include <time/Time.h>
#include <gasmanager/GasManager.h>
#include <decomanager/algorithms/DiveAlgorithm.h>
#include <decomanager/algorithms/buhlmann/BuhlmannAlgorithm.h>
#include <filesystem/JsonSerializable.h>
#include <filesystem/FileSystem.h>
#include <domain/logbook/LogBook.h>

class DecoManager : public JsonSerializable {
public:
    ~DecoManager() override = default;

    void init(FileSystem *fileSystem, uint32_t currentTime);

    void update(uint32_t currentTime, double currentPressureInBar, double tempInCelsius, bool wetContactActivated);

    DecompressionPlan *getDecoPlan();


    // dive
    Dive *getCurrentDive();

    Dive *loadDive(uint16_t diveNr);

    void startDive(uint32_t currentTime);

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
    JsonObject serializeObject(JsonObject &doc) override;

    void deserializeObject(JsonObject &doc) override;

    size_t getFileSize() override;

private:
    FileSystem *_fileSystem;

    LogBook *_logBook;

    Dive *_currentDive;
    DiveAlgorithm *_currentAlgorithm;
    std::list<DiveAlgorithm *> _algorithms;
    GasManager *_gasManager;

    uint32_t _previousUpdateTime;
    double _previousPressureInBar;

};


#endif //DIVEC_DECOMANAGER_H
