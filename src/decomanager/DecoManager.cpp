#include "DecoManager.h"


void DecoManager::init(FileSystem *fileSystem, uint32_t currentTimeInSeconds) {
    Serial.println(F("Initializing deco manager."));

    _fileSystem = fileSystem;

    // init algorithms
    _algorithms[BUHLMANN_B_GF] = new BuhlmannAlgorithm(BuhlmannTable(Constants::BUHLMANN_ZHL16_B_TABLE));
    _algorithms[BUHLMANN_C_GF] = new BuhlmannAlgorithm(BuhlmannTable(Constants::BUHLMANN_ZHL16_C_TABLE));
    _currentAlgorithmIndex = BUHLMANN_B_GF;
    Serial.println(F(" - algorithms initialized."));

    // init deco state and gasses
    fileSystem->loadDecoState(this); // load last deco state -> current algorithm, all algorithms state, _previousUpdateTimeInSeconds, _noFlyTimeInMinutes
    Serial.println(F(" - decostate initialized."));

    // load logbook
    _logBook = new LogBook();
    _logBook->init(_fileSystem); //loads the existing logbook or creates a new one if it didn't exist

    // init dive
    _dive = &Dive::getInstance();
    Serial.println(F(" - dive initialized."));

    // init gasmanager
    _gasManager = new GasManager();
    _gasManager->init(fileSystem);
    Serial.println(F(" - gasses initialized."));

    _previousPressureInBar = Settings::SURFACE_PRESSURE;
    _previousUpdateTimeInSeconds = currentTimeInSeconds;
    Serial.println(F(" - deco manager initialized."));
}

// clears tissues and dive !!!
void DecoManager::reset(uint32_t currentTimeInSeconds) {
    _previousPressureInBar = Settings::SURFACE_PRESSURE;
    _previousUpdateTimeInSeconds = currentTimeInSeconds;
    _dive->init();
    for (auto algorithm:_algorithms) {
        algorithm->reset();
    }
}

void DecoManager::setCurrentAlgorithmIndex(uint8_t currentAlgorithmIndex) {
    _currentAlgorithmIndex = currentAlgorithmIndex;
}

DiveAlgorithm *DecoManager::getCurrentAlgorithm() {
    return _algorithms[_currentAlgorithmIndex];
}

void DecoManager::update(uint32_t currentTimeInSeconds, double currentPressureInBar, double tempInCelsius, bool wetContactActivated) {
/*    Serial.print(F("  - update: time: "));
    Serial.print(DateTime(currentTimeInSeconds).timestamp());
    Serial.print(F(", bar:"));
    Serial.print(currentPressureInBar);
    Serial.print(F(", temp:"));
    Serial.print(tempInCelsius);
    Serial.print(F(", wet contact active:"));
    Serial.println(wetContactActivated);*/

    // auto start dive on activation of the wet contact or if we are below our dive pressure threshold
    if (!_dive->isStarted() && (wetContactActivated || (currentPressureInBar > (Settings::SURFACE_PRESSURE + Settings::START_OF_DIVE_PRESSURE)))) {
        startDive(currentTimeInSeconds);
        Serial.println(F(" -> dive started!"));
    }

    // only update dive and algorithms if the dive is in progress
    if (_dive->isInProgress()) {
        // update dive
        _dive->update(currentTimeInSeconds, _gasManager->getCurrentGas(), currentPressureInBar, tempInCelsius);

        // update logbook
        _logBook->addDiveStep(currentTimeInSeconds, currentPressureInBar, tempInCelsius, _gasManager->getCurrentGas()->getName());

        // update algorithms (all of them (not just the current one) or else we can't switch algorithms while tissues are still loaded)
        for (DiveAlgorithm *diveAlgorithm:_algorithms) {
            diveAlgorithm->update(_previousUpdateTimeInSeconds, currentTimeInSeconds, _gasManager->getCurrentGas(), _previousPressureInBar, currentPressureInBar);
        }

        // dive has just ended
        if (_dive->isEnded()) {
            Serial.println(F(" - dive ended!"));
            endDive();
        }
    }

    _previousUpdateTimeInSeconds = currentTimeInSeconds;
    _previousPressureInBar = currentPressureInBar;
}

DecompressionPlan *DecoManager::getDecoPlan() {
    return getCurrentAlgorithm()->getDecoPlan(_gasManager, _dive->getDiveTimeInSeconds());
}

GasManager *DecoManager::getGasManager() {
    return _gasManager;
}

Gas *DecoManager::getCurrentGas() {
    return getGasManager()->getCurrentGas();
}

Dive *DecoManager::getDive() {
    return _dive;
}

void DecoManager::startDive(uint32_t currentTimeInSeconds) {
    applySurfaceInterval(currentTimeInSeconds);
    _dive->start(currentTimeInSeconds);
}

void DecoManager::endDive() {
    // needed for surface interval
    _lastDiveEndTimeInSeconds = Time::getTime();

    // save the dive log
    _logBook->saveDive(_dive);

    // save current decostate
    _fileSystem->saveDecoState(this);
}

Dive *DecoManager::loadDive(uint16_t diveNr) {
    return _logBook->loadDive(diveNr);
}

uint32_t DecoManager::getNdlInSeconds() {
    return getCurrentAlgorithm()->getNdlInSeconds(_gasManager);
}

uint32_t DecoManager::getSurfaceIntervalInSeconds() const {
    if (_lastDiveEndTimeInSeconds > 0 && !_dive->isInProgress()) {
        return Time::getTime() - _lastDiveEndTimeInSeconds;
    }
    return 0; // if there was no previous dive or a dive is in progress we don't have a surface interval
}

void DecoManager::applySurfaceInterval(uint32_t currentTime) {
    // update algorithms (all of them (not just the current one) or else we can't switch algorithms while tissues are still loaded)
    for (auto *diveAlgorithm:_algorithms) {
        diveAlgorithm->applySurfaceInterval(getSurfaceIntervalInSeconds());
    }
    _lastDiveEndTimeInSeconds = currentTime;
}

uint32_t DecoManager::getPreviousUpdateTime() const {
    return _previousUpdateTimeInSeconds;
}

double DecoManager::getPreviousPressureInBar() const {
    return _previousPressureInBar;
}

const LogBook *DecoManager::getLogBook() const {
    return _logBook;
}

JsonObject DecoManager::serializeObject(JsonObject &doc) {

    doc["_lastDiveEndTimeInSeconds"] = _lastDiveEndTimeInSeconds;
    doc["currentAlgorithm"] = _currentAlgorithmIndex;
    JsonObject algorithmJson = doc.createNestedObject("BUHLMANN_B_GF");
    _algorithms[BUHLMANN_B_GF]->serializeObject(algorithmJson);

    algorithmJson = doc.createNestedObject("BUHLMANN_C_GF");
    _algorithms[BUHLMANN_C_GF]->serializeObject(algorithmJson);

    return doc;
}

void DecoManager::deserializeObject(JsonObject &doc) {
    // load last deco state -> _previousUpdateTimeInSeconds, current algorithm, all algorithms state (mainly tissues)
    _previousUpdateTimeInSeconds = doc["previousUpdateTime"];
    _currentAlgorithmIndex = doc["currentAlgorithm"];

    JsonObject algorithmJson = doc["BUHLMANN_B_GF"];
    _algorithms[BUHLMANN_B_GF]->deserializeObject(algorithmJson);

    algorithmJson = doc["BUHLMANN_C_GF"];
    _algorithms[BUHLMANN_C_GF]->deserializeObject(algorithmJson);
}

size_t DecoManager::getJsonSize() {
    size_t fileSize = JSON_OBJECT_SIZE(2); // previousUpdateTimeInSeconds and currentAlgorithm
    for (auto algorithm:_algorithms) {
        fileSize += algorithm->getJsonSize(); // add sizes of algorithm states
    }
    return fileSize;
}






















