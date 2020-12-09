#include "DiveController.h"

WetContact DiveController::_wetContact;
PiezoButton DiveController::_leftButton;
PiezoButton DiveController::_rightButton;

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

    // init buttons
    _leftButton.init(LEFT_BUTTON_PIN, &onLeftButtonClicked);
    _rightButton.init(RIGHT_BUTTON_PIN, &onRightButtonClicked);

    // init display
    _display.init(&_fileSystem);

    //init deco manager
    _decoManager.init(&_fileSystem, Time::getTime());

    // init menu
    _menu.init(&_display, &_decoManager);

    // init bluetooth
    _bluetooth.init();

    // reset last update time stamp (needs to happen after loading the decomanager to avoid errors in surface interval calculations)
    _lastUpdateTime = Time::getTime();

    Serial.println(F("DiveController ready"));
}

void DiveController::step() {
    DateTime currentTime = Time::getTime();

    // only add a step every STEP_INTERVAL seconds
    if ((currentTime.secondstime() - _lastUpdateTime.secondstime()) >= Settings::STEP_INTERVAL) {

        // update deco manager (dive, tissues, gasses, ...)
        _decoManager.update(currentTime, _depthSensor.pressureInBar(), _depthSensor.tempInCelsius(), _wetContact.isActivated());

        // update menu
        _menu.update();

        _lastUpdateTime = currentTime;
    }

    // check bluetooth - don't wait for interval or a bluetooth command takes 5 seconds to be handled
    int8_t bleCommand = _bluetooth.receive();


    // handle button presses - don't wait for interval or a button press takes 5 seconds to be handled
    if (_leftButton.isPressed() || bleCommand == BLE_PRESS_LEFT_BUTTON) {
        Serial.println(F("Left Button pressed "));
        _menu.handleLeftButtonPress();
    }
    if (_rightButton.isPressed() || bleCommand == BLE_PRESS_RIGHT_BUTTON) {
        Serial.println(F("Right Button pressed "));
        _menu.handleRightButtonPress();
    }

}

void DiveController::onLeftButtonClicked() {
    _leftButton.onButtonClicked();
}

void DiveController::onRightButtonClicked() {
    _rightButton.onButtonClicked();
}

void DiveController::onWetContactChanged() {
    _wetContact.onChange();
}

const DecoManager &DiveController::getDecoManager() const {
    return _decoManager;
}

const FileSystem &DiveController::getFileSystem() const {
    return _fileSystem;
}












