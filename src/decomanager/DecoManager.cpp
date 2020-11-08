#include "DecoManager.h"

void DecoManager::init(FileSystem *fileSystem, const DateTime &currentTime) {
    Serial.println(F("Initializing deco manager."));

    // init algorithms
    addAlgorithm(new BuhlmannAlgorithm("BUHLMANN_B_GF", BuhlmannTable(Constants::BUHLMANN_ZHL16_B_TABLE)));
    addAlgorithm(new BuhlmannAlgorithm("BUHLMANN_C_GF", BuhlmannTable(Constants::BUHLMANN_ZHL16_C_TABLE)));
    setCurrentAlgorithm("BUHLMANN_C_GF");
    Serial.println(F(" - algorithms initialized."));

    // init deco state and gasses
    fileSystem->loadDecoState(this); // load last deco state -> current algorithm, all algorithms state, _previousUpdateTimeInSeconds, _noFlyTimeInMinutes
    Serial.println(F(" - decostate initialized."));

    // init dive
    _currentDive = new Dive();
    _currentDive->init();
    Serial.println(F(" - dive initialized."));

    // init gasmanager
    _gasManager = new GasManager();
    _gasManager->init(fileSystem);
    Serial.println(F(" - gasses initialized."));

    // add surface interval if there was a previous state
    if ((currentTime.secondstime() - _previousUpdateTime.secondstime()) > 5) {
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

void DecoManager::update(const DateTime &currentTime, double currentPressureInBar, double tempInCelsius, bool wetContactActivated) {
    // auto start dive on activation of the wet contact or if we are below our dive pressure threshold
    if (!_currentDive->isStarted() && (wetContactActivated || (currentPressureInBar > (Settings::SURFACE_PRESSURE + Settings::START_OF_DIVE_PRESSURE)))) {
        _currentDive->start(currentTime);
        Serial.println(F(" -> dive started!"));
    }

    // only update dive and algorithms if the dive is in progress
    if (_currentDive->isInProgress()) {
        _currentDive->update(currentTime, _gasManager->getCurrentOcGas(), currentPressureInBar, tempInCelsius);

        // update algorithms (all of them (not just the current one) or else we can't switch algorithms while tissues are still loaded)
        for (DiveAlgorithm *diveAlgorithm:_algorithms) {
            diveAlgorithm->update(_previousUpdateTime.secondstime(), currentTime.secondstime(), _gasManager, _previousPressureInBar, currentPressureInBar);
        }
        // dive has just ended
        if (_currentDive->isEnded()) {
            Serial.println(F(" - dive ended!"));
            // TODO: LOG DIVE
            _currentDive->init(); // initializes a new dive (this new dive is not started yet. it is waiting till "start" is being called to start taking updates)
        }
    }

    _previousUpdateTime = currentTime;
    _previousPressureInBar = currentPressureInBar;
}

DecompressionPlan *DecoManager::getDecoPlan() {
    DecompressionPlan *plan = getCurrentAlgorithm()->getDecoPlan(_gasManager);
    // add dive steps

    return plan;
}

void DecoManager::addSurfaceInterval(const DateTime &beginTime, const DateTime &endTime) {
    Serial.print(F(" - adding surface interval: "));
    Serial.print(endTime.secondstime() - beginTime.secondstime());
    Serial.println(F(" (s)"));

    _gasManager->setCurrentOcGas(&GasManager::AIR);
    // update algorithms (all of them (not just the current one) or else we can't switch algorithms while tissues are still loaded)
    for (DiveAlgorithm *diveAlgorithm:_algorithms) {
        diveAlgorithm->update(beginTime.secondstime(), endTime.secondstime(), _gasManager, Settings::SURFACE_PRESSURE, Settings::SURFACE_PRESSURE);
    }
}

GasManager *DecoManager::getGasManager() {
    return _gasManager;
}

Dive *DecoManager::getCurrentDive() {
    return _currentDive;
}

uint32_t DecoManager::getNdlInSeconds() {
    return _currentAlgorithm->getNdlInSeconds();
}

uint32_t DecoManager::getSurfaceIntervalInSeconds() {
    if (!_currentDive->isInProgress()) {
        return Time::getTime().secondstime() - _previousUpdateTime.secondstime();
    }
    return 0;
}

const DateTime &DecoManager::getPreviousUpdateTime() const {
    return _previousUpdateTime;
}

double DecoManager::getPreviousPressureInBar() const {
    return _previousPressureInBar;
}

uint32_t DecoManager::getNoFlyTimeInMinutes() const {
    return _noFlyTimeInMinutes;
}

size_t DecoManager::serialize(File *file) {
    DynamicJsonDocument doc(getFileSize());

    doc["previousUpdateTime"] = _previousUpdateTime.secondstime();
    doc["noFlyTimeInMinutes"] = _noFlyTimeInMinutes;
    doc["currentAlgorithm"] = _currentAlgorithm->getName();
    for (auto algorithm:_algorithms) {
        JsonObject algorithmJson = doc.createNestedObject(algorithm->getName());
        algorithm->serialize(algorithmJson);
    }

    return serializeJsonPretty(doc, *file);
}

DeserializationError DecoManager::deserialize(File *file) {
    DynamicJsonDocument doc(getFileSize());

    DeserializationError error = deserializeJson(doc, *file);
    if (error) { // stop deserializing if json parse failed
        return error;
    }

    // load last deco state -> _previousUpdateTime, current algorithm, all algorithms state (mainly tissues)
    uint32_t timeInSeconds = doc["previousUpdateTime"];
    _previousUpdateTime = DateTime(timeInSeconds);
    setCurrentAlgorithm(doc["currentAlgorithm"]);
    for (auto algorithm:_algorithms) {
        JsonObject algorithmJson = doc[algorithm->getName()];
        algorithm->deserialize(algorithmJson);
    }

    //Serial.println("Loaded decostate.");
    //serializeJsonPretty(doc, Serial);
    //Serial.println();

    return error;
}

size_t DecoManager::getFileSize() {
    size_t fileSize = JSON_OBJECT_SIZE(3); // previousUpdateTimeInSeconds,noFlyTimeInMinutes and currentAlgorithm
    for (auto algorithm:_algorithms) {
        fileSize += algorithm->getObjectSize(); // add sizes of algorithm states
    }
    return fileSize + 500; // TODO: why out of memory without +500??????????????
}













