#include "DecoManager.h"

void DecoManager::init(FileSystem *fileSystem, uint32_t currentTimeInSeconds) {
    Serial.println(F("Initializing deco manager."));

    _fileSystem = fileSystem;

    // init algorithms
    //addAlgorithm(new BuhlmannAlgorithm("BUHLMANN_A_GF", BuhlmannTable(Constants::BUHLMANN_ZHL16_A_TABLE)));
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
    _dive = &Dive::getInstance();
    Serial.println(F(" - dive initialized."));

    // init gasmanager
    _gasManager = new GasManager();
    _gasManager->init(fileSystem);
    Serial.println(F(" - gasses initialized."));

    // add surface interval if there was a previous state
    if ((currentTimeInSeconds - _previousUpdateTimeInSeconds) > 5) {
        addSurfaceInterval(_previousUpdateTimeInSeconds, currentTimeInSeconds);
    }

    _previousPressureInBar = Settings::SURFACE_PRESSURE;
    _previousUpdateTimeInSeconds = currentTimeInSeconds;
    Serial.println(F(" - deco manager initialized."));
}

void DecoManager::reset(uint32_t currentTimeInSeconds) {
    _previousPressureInBar = Settings::SURFACE_PRESSURE;
    _previousUpdateTimeInSeconds = currentTimeInSeconds;
    _dive->init();
    for (auto algorithm:_algorithms) {
        algorithm->reset();
    }
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

void DecoManager::update(uint32_t currentTimeInSeconds, double currentPressureInBar, double tempInCelsius, bool wetContactActivated) {
    Serial.print(F("  - update: time: "));
    Serial.print(DateTime(currentTimeInSeconds).timestamp());
    Serial.print(F(", bar:"));
    Serial.print(currentPressureInBar);
    Serial.print(F(", temp:"));
    Serial.print(tempInCelsius);
    Serial.print(F(", wet contact active:"));
    Serial.println(wetContactActivated);
    // auto start dive on activation of the wet contact or if we are below our dive pressure threshold
    if (!_dive->isStarted() && (wetContactActivated || (currentPressureInBar > (Settings::SURFACE_PRESSURE + Settings::START_OF_DIVE_PRESSURE)))) {
        startDive(currentTimeInSeconds);
        Serial.println(F(" -> dive started!"));
    }

    // only update dive and algorithms if the dive is in progress
    if (_dive->isInProgress()) {
        // update dive
        auto step = _dive->update(currentTimeInSeconds, _gasManager->getCurrentOcGas(), currentPressureInBar, tempInCelsius);

        // update logbook (avoid logging steps when we are on the surface - dive could be in progress as we continue the dive if the diver goes under again within the time limit)
        if (!_dive->isSurfaced()) {
            _logBook->addDiveStep(step);
        }

        // update algorithms (all of them (not just the current one) or else we can't switch algorithms while tissues are still loaded)
        for (DiveAlgorithm *diveAlgorithm:_algorithms) {
            diveAlgorithm->update(_previousUpdateTimeInSeconds, currentTimeInSeconds, _gasManager, _previousPressureInBar, currentPressureInBar);
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

Dive *DecoManager::getDive() {
    return _dive;
}

void DecoManager::startDive(uint32_t currentTimeInSeconds) {
    _dive->start(currentTimeInSeconds);
    _logBook->initTmpDiveLog();
}

void DecoManager::endDive() {
    // save the dive log
    _logBook->saveDive(_dive);

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
    if (!_dive->isInProgress()) {
        return Time::getTime() - _previousUpdateTimeInSeconds;
    }
    return 0;
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

    doc["previousUpdateTime"] = getPreviousUpdateTime();
    doc["currentAlgorithm"] = _currentAlgorithm->getName();
    for (auto algorithm:_algorithms) {
        JsonObject algorithmJson = doc.createNestedObject(algorithm->getName());
        algorithm->serializeObject(algorithmJson);
    }

    return doc;
}

void DecoManager::deserializeObject(JsonObject &doc) {
    // load last deco state -> _previousUpdateTimeInSeconds, current algorithm, all algorithms state (mainly tissues)
    _previousUpdateTimeInSeconds = doc["previousUpdateTime"];
    setCurrentAlgorithm(doc["currentAlgorithm"]);

    for (auto algorithm:_algorithms) {
        JsonObject algorithmJson = doc[algorithm->getName()];
        algorithm->deserializeObject(algorithmJson);
    }
}

size_t DecoManager::getJsonSize() {
    size_t fileSize = JSON_OBJECT_SIZE(2); // previousUpdateTimeInSeconds and currentAlgorithm
    for (auto algorithm:_algorithms) {
        fileSize += algorithm->getJsonSize(); // add sizes of algorithm states
    }
    return fileSize;
}




















