#ifndef DIVEC_DIVE_CONTROLLER_H
#define DIVEC_DIVE_CONTROLLER_H


#include <time/Time.h>
#include <menu/Menu.h>
#include <gasmanager/GasManager.h>
#include <sensors/depth/DepthSensor.h>
#include <sensors/wetcontact/WetContact.h>
#include <sensors/button/PiezoButton.h>

#define LEFT_BUTTON_PIN A1
#define RIGHT_BUTTON_PIN A2
#define WET_CONTACT_PIN 10

#define USE_MOCK_DEPTH_SENSOR false
#define USE_MOCK_TIME false

class DiveController {
public:

    void setup();

    void step();

    const FileSystem &getFileSystem() const;

    const DecoManager &getDecoManager() const;

    const Settings &getSettings() const;

    const Time &getTime() const;

    const DepthSensor &getDepthSensor() const;

    const Display &getDisplay() const;

    const Menu &getMenu() const;

    const DateTime &getLastUpdateTime() const;

    static const WetContact &getWetContact();

private:
    FileSystem _fileSystem;
    DecoManager _decoManager;

    Settings _settings;

    DepthSensor _depthSensor;

    Display _display;
    Menu _menu;

    DateTime _lastUpdateTime;

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