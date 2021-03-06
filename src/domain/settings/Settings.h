#ifndef DIVEC_SETTINGS_H
#define DIVEC_SETTINGS_H

#include <filesystem/FileSystem.h>
#include <Arduino.h>
#include <domain/settings/Constants.h>

#define SALT 0
#define FRESH 1

#define BUHLMANN_B_GF 0
#define BUHLMANN_C_GF 1

const char SPACER_15[] PROGMEM = "               ";
const char SPACER_10[] PROGMEM = "          ";
const char SPACER_2[]  PROGMEM = "  ";

class Settings final : public JsonSerializable {
public:

    // init
    void init(FileSystem *fileSystem);

    // algorithm selection
    static uint8_t ALGORITHM;                 // BUHLMANN_ZHL16B_GF (0)/ BUHLMANN_ZHL16C_GF (1)

    // buhlmann settings
    static double GF_LOW;                     // gradient factor low
    static double GF_HIGH;                    // gradient factor high

    // deco settings
    static uint8_t MIN_STOP_TIME;             // minimum stop time intervals in seconds
    static uint8_t DECO_STEP_SIZE;            // meters between decostops
    static uint8_t LAST_STOP;                 // meters. meters below surface to make the last decostop
    static uint8_t SALINITY;                  // SALT(1)/FRESH(2)
    static double SURFACE_PRESSURE;           // bar. default is pressure on the surface (aka barometric pressure). should be measured and updated at start of dive
    static double MAX_PPO2;                   // bar.
    static uint16_t MAX_END;                  // meters. max depth while using standard air (79% N2)
    static bool O2_NARCOTIC;                  // consider O2 as narcotic during END calculation.

    // controller settings
    static uint8_t STEP_INTERVAL;             // seconds between measurements
    static double START_OF_DIVE_PRESSURE;     // bars. minimal pressure above surface pressure before dive start
    static double END_OF_DIVE_PRESSURE;       // bars. pressure above surface pressure considered to be "on the surface"
    static uint16_t END_OF_DIVE_DELAY;        // maximum number of seconds on the surface before the dive ends (less than this is considered the same dive)

    // display settings
    static uint16_t TITLE_COLOR;
    static uint16_t VALUE_COLOR;
    static uint16_t ERROR_COLOR;
    static uint16_t WARNING_COLOR;

    static double getWaterPressure();

    // log
    static void logSettings();

    static void logSettings(Print *print);

    // persistence
    JsonObject serializeObject(JsonObject &doc) final;

    void deserializeObject(JsonObject &doc) final;

    size_t getJsonSize() final;

};

#endif