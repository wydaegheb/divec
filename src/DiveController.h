#ifndef DIVEC_DIVE_CONTROLLER_H
#define DIVEC_DIVE_CONTROLLER_H


#include <time/Time.h>
#include <menu/Menu.h>
#include <gasmanager/GasManager.h>
#include <sensors/depth/DepthSensor.h>
#include <sensors/wetcontact/WetContact.h>
#include <sensors/button/PiezoButton.h>
#include <bluetooth/Bluetooth.h>

#define RIGHT_BUTTON_PIN A2
#define LEFT_BUTTON_PIN A3
#define WET_CONTACT_PIN 11

#define USE_MOCK_DEPTH_SENSOR false
#define USE_MOCK_TIME false


class DiveController {
public:

    void setup();

    void step();

    const DecoManager &getDecoManager() const;

private:
    FileSystem _fileSystem;

    DecoManager _decoManager;

    Settings _settings;

    DepthSensor _depthSensor;

    Bluetooth _bluetooth;

    Display _display;
    Menu _menu;

    uint32_t _lastUpdateTimeInSeconds;

    // wet contact interrupt
    static WetContact _wetContact;

    static void onWetContactChanged();

    // button interrupts
    static PiezoButton _leftButton;
    static PiezoButton _rightButton;

    static void onLeftButtonClicked();

    static void onRightButtonClicked();

};

#endif