#ifndef DIVEC_DIVE_CONTROLLER_H
#define DIVEC_DIVE_CONTROLLER_H

// see pins_arduino.h for the definition of the display and sd pins (repeated here in comment to get a complete overview of pins used)
// #define TF_CS   4  // TF (Micro SD Card) CS pin
// #define TS_CS   12 // Touch Screen CS pin
// #define TFT_CS  14 // TFT CS pin
// #define TFT_LED 32 // TFT backlight control pin
// #define TFT_RST 33 // TFT reset pin
// #define TFT_DC  27 // TFT DC pin
// #define SS      TF_CS



#define WET_CONTACT_PIN 0
#define NEXT_BUTTON_PIN 2
#define OK_BUTTON_PIN 15

#define USE_MOCK_DEPTH_SENSOR true
#define USE_MOCK_TIME false


#include <BaseRenderers.h>

#include <time/Time.h>
#include <gasmanager/GasManager.h>
#include <sensors/depth/DepthSensor.h>
#include <sensors/wetcontact/WetContact.h>
#include <decomanager/DecoManager.h>
#include <display/Display.h>
#include <display/pages/MainPage.h>
#include <menu/Menu.h>


// defines the app info to the linker.
const PROGMEM ConnectorLocalInfo applicationInfo = {"DiveC", "3ae2524b-d15c-4736-9c50-64aa6679f66b"};

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

    Display _display;

    Menu _menu;

    uint32_t _lastUpdateTimeInSeconds;

    // wet contact interrupt
    static WetContact _wetContact;

    static void onWetContactChanged();

};

#endif