#ifndef DIVEC_DECOMANAGER_H
#define DIVEC_DECOMANAGER_H

#include <time/Time.h>
#include <gasmanager/GasManager.h>
#include <decomanager/algorithms/DiveAlgorithm.h>
#include <decomanager/algorithms/buhlmann/BuhlmannAlgorithm.h>
#include <filesystem/JsonSerializable.h>
#include <filesystem/FileSystem.h>

class DecoManager : public JsonSerializable {
public:
    ~DecoManager() override = default;

    void init(FileSystem *fileSystem, const DateTime &currentTime);


    void update(const DateTime &currentTime, double currentPressureInBar, double tempInCelsius, bool wetContactActivated);

    DecompressionPlan *getDecoPlan();

    void setCurrentAlgorithm(char const *namename);

    DiveAlgorithm *getCurrentAlgorithm();

    void addAlgorithm(DiveAlgorithm *diveAlgorithm);

    Dive *getCurrentDive();

    GasManager *getGasManager();

    Gas *getCurrentGas();

    uint32_t getNdlInSeconds();

    void addSurfaceInterval(const DateTime &beginTime, const DateTime &endTime);

    uint32_t getSurfaceIntervalInSeconds();

    const DateTime &getPreviousUpdateTime() const;

    double getPreviousPressureInBar() const;

    // persistence
    size_t serialize(File *file) override;

    DeserializationError deserialize(File *file) override;

    size_t getFileSize() override;

private:
    FileSystem *_fileSystem;

    Dive *_currentDive;
    DiveAlgorithm *_currentAlgorithm;
    std::list<DiveAlgorithm *> _algorithms;
    GasManager *_gasManager;

    DateTime _previousUpdateTime;
    double _previousPressureInBar;

};


#endif //DIVEC_DECOMANAGER_H
