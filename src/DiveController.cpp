#include "DiveController.h"

WetContact DiveController::_wetContact;

void DiveController::setup() {
    Serial.println(F("Initiating DiveController."));

    // init file system (have to do this first as other initialisations are using it)
    _fileSystem.init();

    // init settings
    _settings.init(&_fileSystem);

    // init time
    Time::init(USE_MOCK_TIME);

    // init depth sensor
    _depthSensor.init(USE_MOCK_DEPTH_SENSOR);

    // init wet contact
    _wetContact.init(WET_CONTACT_PIN, &onWetContactChanged);

    // init display
    _display.init();

    //init deco manager
    _decoManager.init(&_fileSystem, Time::getTime());

    // init bluetooth
    _bluetooth.init();

    // reset last update time stamp
    _lastUpdateTimeInSeconds = Time::getTime();

    // init menu
    _menu.init(&_display, &_decoManager);

    Serial.println(F("DiveController ready"));
}

void DiveController::step() {
    taskManager.runLoop();

    uint32_t currentTimeInSeconds = Time::getTime();

    // only add a step every STEP_INTERVAL seconds
    if ((currentTimeInSeconds - _lastUpdateTimeInSeconds) >= Settings::STEP_INTERVAL) {

        // update deco manager (dive, tissues, gasses, ...)
        _decoManager.update(currentTimeInSeconds, _depthSensor.pressureInBar(), _depthSensor.tempInCelsius(), _wetContact.isActivated());

        _lastUpdateTimeInSeconds = currentTimeInSeconds;
    }

    // check bluetooth - don't wait for interval or a bluetooth command takes 5 seconds to be handled
    int8_t bleCommand = _bluetooth.receive();

    if (_depthSensor.isMocked() && bleCommand == BLE_INC_TIME_ONE_MIN) {
        Time::incMockTime();
    }

    if (_depthSensor.isMocked() && bleCommand == BLE_ASCENT_ONE_METER) {
        _depthSensor.decreaseMockDepth();
    }

    if (_depthSensor.isMocked() && bleCommand == BLE_DESCENT_ONE_METER) {
        _depthSensor.increaseMockDepth();
    }
}

void DiveController::onWetContactChanged() {
    _wetContact.onChange();
}

const DecoManager &DiveController::getDecoManager() const {
    return _decoManager;
}














