#include "DiveController.h"

WetContact DiveController::_wetContact;

void DiveController::setup() {
    Serial.println(F("Initiating DiveController."));

    // Start SPI
    SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI);

    // init display
    _display.init();

    // init file system
    if (!_fileSystem.init()) {
        _display.drawSystemError("[Filesystem failed]");
        exit(1);
    }

    // init settings
    _settings.init(&_fileSystem);

    // init time
    if (!Time::init(USE_MOCK_TIME)) {
        _display.drawSystemError("[Real Time Clock not found]");
        exit(1);
    }

    // init depth sensor
    if (!_depthSensor.init(USE_MOCK_DEPTH_SENSOR)) {
        _display.drawSystemError("[Depth sensor failed]");
        exit(1);
    }

    // init wet contact
    _wetContact.init(WET_CONTACT_PIN, &onWetContactChanged);

    //init deco manager
    _decoManager.init(&_fileSystem);

    // init menu
    _menu.init(OK_BUTTON_PIN, NEXT_BUTTON_PIN, &_display, &_decoManager);

    // reset last update time stamp
    _lastUpdateTimeInSeconds = Time::getTime();

    Serial.println(F("DiveController ready"));
}

void DiveController::step() {
    taskManager.runLoop();

    uint32_t currentTimeInSeconds = Time::getTime();

    // only add a step every STEP_INTERVAL seconds
    if ((currentTimeInSeconds - _lastUpdateTimeInSeconds) >= Settings::STEP_INTERVAL) {

        // update deco manager (dive, tissues, gasses, ...)
        _decoManager.update(currentTimeInSeconds, _depthSensor.pressureInBar(), _depthSensor.tempInCelsius(),
                            _wetContact.isActivated());

        _lastUpdateTimeInSeconds = currentTimeInSeconds;
    }
}

void DiveController::onWetContactChanged() {
    _wetContact.onChange();
}

const DecoManager &DiveController::getDecoManager() const {
    return _decoManager;
}














