#ifndef DIVEC_DECOMANAGER_H
#define DIVEC_DECOMANAGER_H

#include <gasmanager/GasManager.h>
#include <decomanager/algorithms/DiveAlgorithm.h>
#include <decomanager/algorithms/buhlmann/BuhlmannAlgorithm.h>
#include <filesystem/JsonSerializable.h>
#include <filesystem/FileSystem.h>
#include <domain/logbook/LogBook.h>

class DecoManager final : public JsonSerializable {
public:
    DecoManager() = default;

    void init(FileSystem *fileSystem); // this code would normally go into the constructor but then Serial.begin() is not yet called -> program locks up when doing Serial.print in constructor

    // return a list of decompression stops (or an empty list if there are none)
    DecompressionPlan *getDecoPlan();

    // deco (tissues changing methods)
    void update(uint32_t currentTimeInSeconds, double currentPressureInBar, double tempInCelsius, bool wetContactActivated);

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


    // decompression state
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
