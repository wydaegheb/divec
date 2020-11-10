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
    //_bluetooth.init();

    // reset last update time stamp (needs to happen after loading the decomanager to avoid errors in surface interval calculations)
    _lastUpdateTime = Time::getTime();

    Serial.println(F("DiveController ready"));
}

void DiveController::step() {
    DateTime currentTime = Time::getTime();

    if ((currentTime.secondstime() - _lastUpdateTime.secondstime()) >= Settings::STEP_INTERVAL) { // add a step every STEP_INTERVAL
        // read current pressure and temperature
        _depthSensor.read();
        double pressureInBar = _depthSensor.pressureInBar();
        double tempInCelsius = _depthSensor.tempInCelsius();

        // log
        Serial.print(F("DiveController step. Time:"));
        Serial.print(currentTime.timestamp(DateTime::TIMESTAMP_TIME));
        Serial.print(F(" - Pressure:"));
        Serial.print(pressureInBar);
        Serial.print(F(" bar - Temp:"));
        Serial.print(tempInCelsius);
        Serial.println(F(" celsius"));

        // update deco manager (dive, tissues, gasses, ...)
        _decoManager.update(currentTime, pressureInBar, tempInCelsius, _wetContact.isActivated());

        // update menu
        _menu.update();


        _lastUpdateTime = currentTime;
    }

    // check bluetooth - don't wait for interval or a bluetooth command takes 5 seconds to be handled
    //_bluetooth.receive();

    // handle button presses - don't wait for interval or a button press takes 5 seconds to be handled
    if (_leftButton.isPressed()) {
        Serial.println(F("Left Button pressed "));
        _menu.handleLeftButtonPress();
    }
    if (_rightButton.isPressed()) {
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

const FileSystem &DiveController::getFileSystem() const {
    return _fileSystem;
}

const DecoManager &DiveController::getDecoManager() const {
    return _decoManager;
}

const Settings &DiveController::getSettings() const {
    return _settings;
}

const DepthSensor &DiveController::getDepthSensor() const {
    return _depthSensor;
}

const Display &DiveController::getDisplay() const {
    return _display;
}

const Menu &DiveController::getMenu() const {
    return _menu;
}

const DateTime &DiveController::getLastUpdateTime() const {
    return _lastUpdateTime;
}

const WetContact &DiveController::getWetContact() {
    return _wetContact;
}








