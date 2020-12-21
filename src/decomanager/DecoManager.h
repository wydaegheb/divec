#ifndef DIVEC_DECOMANAGER_H
#define DIVEC_DECOMANAGER_H

#include <gasmanager/GasManager.h>
#include <decomanager/algorithms/DiveAlgorithm.h>
#include <decomanager/algorithms/buhlmann/BuhlmannAlgorithm.h>
#include <filesystem/JsonSerializable.h>
#include <filesystem/FileSystem.h>
#include <domain/logbook/LogBook.h>

#define BUHLMANN_B_GF 0
#define BUHLMANN_C_GF 1

class DecoManager final : public JsonSerializable {
public:

    void init(FileSystem *fileSystem, uint32_t currentTimeInSeconds);

    // deco (tissues changing methods)
    void update(uint32_t currentTimeInSeconds, double currentPressureInBar, double tempInCelsius, bool wetContactActivated);

    DecompressionPlan *getDecoPlan();

    void reset(uint32_t currentTimeInSeconds); // reset all - CLEARS tissues!


    // dive
    Dive *getDive();

    Dive *loadDive(uint16_t diveNr);

    void startDive(uint32_t currentTimeInSeconds);

    void endDive();

    const LogBook *getLogBook() const;


    // algorithm
    void setCurrentAlgorithmIndex(uint8_t currentAlgorithmIndex);

    DiveAlgorithm *getCurrentAlgorithm();


    // gas
    GasManager *getGasManager();

    Gas *getCurrentGas();


    // state
    uint32_t getNdlInSeconds();

    uint32_t getSurfaceIntervalInSeconds() const;

    uint32_t getPreviousUpdateTime() const;

    double getPreviousPressureInBar() const;


    // persistence
    JsonObject serializeObject(JsonObject &doc) final;

    void deserializeObject(JsonObject &doc) final;

    size_t getJsonSize() final;

private:
    FileSystem *_fileSystem;

    LogBook *_logBook;

    Dive *_dive;
    uint8_t _currentAlgorithmIndex;
    DiveAlgorithm *_algorithms[2]; // extend this if new algorithms are added

    GasManager *_gasManager;

    uint32_t _previousUpdateTimeInSeconds;
    uint32_t _lastDiveEndTimeInSeconds;

    double _previousPressureInBar;

    // surface interval
    void applySurfaceInterval(uint32_t currentTime);

};


#endif //DIVEC_DECOMANAGER_H
