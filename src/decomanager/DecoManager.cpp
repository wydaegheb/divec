#include "DecoManager.h"

void DecoManager::init(FileSystem *fileSystem, uint32_t currentTime) {
    Serial.println(F("Initializing deco manager."));

    _fileSystem = fileSystem;

    // init algorithms
    addAlgorithm(new BuhlmannAlgorithm("BUHLMANN_B_GF", BuhlmannTable(Constants::BUHLMANN_ZHL16_B_TABLE)));
    addAlgorithm(new BuhlmannAlgorithm("BUHLMANN_C_GF", BuhlmannTable(Constants::BUHLMANN_ZHL16_C_TABLE)));
    setCurrentAlgorithm("BUHLMANN_C_GF");
    Serial.println(F(" - algorithms initialized."));

    // init deco state and gasses
    fileSystem->loadDecoState(this); // load last deco state -> current algorithm, all algorithms state, _previousUpdateTimeInSeconds, _noFlyTimeInMinutes
    Serial.println(F(" - decostate initialized."));

    // load logbook
    _logBook = new LogBook();
    _logBook->init(_fileSystem); //loads the existing logbook or creates a new one if it didn't exist

    // init dive
    _currentDive = new Dive();
    Serial.println(F(" - dive initialized."));

    // init gasmanager
    _gasManager = new GasManager();
    _gasManager->init(fileSystem);
    Serial.println(F(" - gasses initialized."));

    // add surface interval if there was a previous state
    if ((currentTime - _previousUpdateTime) > 5) {
        addSurfaceInterval(_previousUpdateTime, currentTime);
    }

    _previousPressureInBar = Settings::SURFACE_PRESSURE;
    _previousUpdateTime = currentTime;
    Serial.println(F(" - deco manager initialized."));
}


void DecoManager::setCurrentAlgorithm(char const *name) {
    _currentAlgorithm = _algorithms.front();
    for (DiveAlgorithm *diveAlgorithm:_algorithms) {
        if (strcmp(diveAlgorithm->getName(), name) == 0) {
            _currentAlgorithm = diveAlgorithm;
        }
    }
}

DiveAlgorithm *DecoManager::getCurrentAlgorithm() {
    return _currentAlgorithm;
}

void DecoManager::addAlgorithm(DiveAlgorithm *diveAlgorithm) {
    _algorithms.push_back(diveAlgorithm);
}

void DecoManager::update(uint32_t currentTime, double currentPressureInBar, double tempInCelsius, bool wetContactActivated) {
    // auto start dive on activation of the wet contact or if we are below our dive pressure threshold
    if (!_currentDive->isStarted() && (wetContactActivated || (currentPressureInBar > (Settings::SURFACE_PRESSURE + Settings::START_OF_DIVE_PRESSURE)))) {
        startDive(currentTime);
        Serial.println(F(" -> dive started!"));
    }

    // only update dive and algorithms if the dive is in progress
    if (_currentDive->isInProgress()) {
        // update dive
        auto step = _currentDive->update(currentTime, _gasManager->getCurrentOcGas(), currentPressureInBar, tempInCelsius);

        // update logbook (avoid logging steps when we are on the surface - dive could be in progress as we continue the dive if the diver goes under again within the time limit)
        if (!_currentDive->isSurfaced()) {
            _logBook->addDiveStep(step);
        }

        // update algorithms (all of them (not just the current one) or else we can't switch algorithms while tissues are still loaded)
        for (DiveAlgorithm *diveAlgorithm:_algorithms) {
            diveAlgorithm->update(_previousUpdateTime, currentTime, _gasManager, _previousPressureInBar, currentPressureInBar);
        }

        // dive has just ended
        if (_currentDive->isEnded()) {
            Serial.println(F(" - dive ended!"));
            endDive();
        }
    }

    _previousUpdateTime = currentTime;
    _previousPressureInBar = currentPressureInBar;
}

DecompressionPlan *DecoManager::getDecoPlan() {
    return getCurrentAlgorithm()->getDecoPlan(_gasManager);
}

void DecoManager::addSurfaceInterval(uint32_t beginTime, uint32_t endTime) {
    Serial.print(F(" - adding surface interval: "));
    Serial.print(endTime - beginTime);
    Serial.println(F(" (s)"));

    _gasManager->setCurrentOcGas(&GasManager::AIR);
    // update algorithms (all of them (not just the current one) or else we can't switch algorithms while tissues are still loaded)
    for (DiveAlgorithm *diveAlgorithm:_algorithms) {
        diveAlgorithm->update(beginTime, endTime, _gasManager, Settings::SURFACE_PRESSURE, Settings::SURFACE_PRESSURE);
    }
}

GasManager *DecoManager::getGasManager() {
    return _gasManager;
}

Gas *DecoManager::getCurrentGas() {
    return getGasManager()->getCurrentOcGas();
}

Dive *DecoManager::getCurrentDive() {
    return _currentDive;
}

void DecoManager::startDive(uint32_t currentTime) {
    _currentDive->start(currentTime);
    _logBook->initTmpDiveLog();
}

void DecoManager::endDive() {
    // save the dive log
    _logBook->saveDive(_currentDive);

    // save current decostate
    _fileSystem->saveDecoState(this);
}

Dive *DecoManager::loadDive(uint16_t diveNr) {
    return _logBook->loadDive(diveNr);
}

uint32_t DecoManager::getNdlInSeconds() {
    return _currentAlgorithm->getNdlInSeconds(_gasManager);
}

uint32_t DecoManager::getSurfaceIntervalInSeconds() {
    if (!_currentDive->isInProgress()) {
        return Time::getTime() - _previousUpdateTime;
    }
    return 0;
}

uint32_t DecoManager::getPreviousUpdateTime() const {
    return _previousUpdateTime;
}

double DecoManager::getPreviousPressureInBar() const {
    return _previousPressureInBar;
}

const LogBook *DecoManager::getLogBook() const {
    return _logBook;
}

JsonObject DecoManager::serializeObject(JsonObject &doc) {

    doc["previousUpdateTime"] = getPreviousUpdateTime();
    doc["currentAlgorithm"] = _currentAlgorithm->getName();
    for (auto algorithm:_algorithms) {
        JsonObject algorithmJson = doc.createNestedObject(algorithm->getName());
        algorithm->serialize(algorithmJson);
    }

    return doc;
}

void DecoManager::deserializeObject(JsonObject &doc) {
    // load last deco state -> _previousUpdateTime, current algorithm, all algorithms state (mainly tissues)
    _previousUpdateTime = doc["previousUpdateTime"];
    setCurrentAlgorithm(doc["currentAlgorithm"]);
    for (auto algorithm:_algorithms) {
        JsonObject algorithmJson = doc[algorithm->getName()];
        algorithm->deserialize(algorithmJson);
    }
}

size_t DecoManager::getFileSize() {
    size_t fileSize = JSON_OBJECT_SIZE(3); // previousUpdateTimeInSeconds,noFlyTimeInMinutes and currentAlgorithm
    for (auto algorithm:_algorithms) {
        fileSize += algorithm->getObjectSize(); // add sizes of algorithm states
    }
    return fileSize;
}


















