#include "Settings.h"

// algorithm selection
uint8_t Settings::ALGORITHM = BUHLMANN_C_GF;        // BUHLMANN_ZHL16B_GF (0)/ BUHLMANN_ZHL16C_GF (1)

// buhlmann settings
double Settings::GF_LOW = 1.0;                      // gradient factor low
double Settings::GF_HIGH = 1.0;                     // gradient factor high

// deco settings
uint8_t Settings::MIN_STOP_TIME = 60;               // minimum stop time intervals in seconds
uint8_t Settings::DECO_STEP_SIZE = 3;               // meters between decostops
uint8_t Settings::LAST_STOP = 6;                    // meters. meters below surface to make the last decostop
uint8_t Settings::SALINITY = SALT;                  // SALT/FRESH
double Settings::SURFACE_PRESSURE = 1.0;            // bar. default is pressure on the surface. (TODO: should be measured and updated at start of dive)
double Settings::MAX_PPO2 = 1.4;                    // bar. (Typically 1.4 for bottom minus 0.5 for each extra difficulty (cold, current, stress, age,...) and 1.6 while doing deco stops)
uint16_t Settings::MAX_END = 40;                    // meters. max depth while using standard air (79% N2 - 0% He) - (the pO2 becomes > 1.4 below 56,6m but most dive organisations consider deeper than 40m as unsafe due to N2 narcosis)
bool Settings::O2_NARCOTIC = true;                  // consider O2 as narcotic during END calculation.

// controller settings
uint8_t Settings::STEP_INTERVAL = 3;                // seconds between measurements
double Settings::START_OF_DIVE_PRESSURE = 0.25;     // bars. minimal pressure above surface pressure before dive start (i.e. "start dive pressure")
double Settings::END_OF_DIVE_PRESSURE = 0.1;        // bars. pressure above surface pressure considered to be "on the surface" (i.e. "stop dive pressure")
uint16_t Settings::END_OF_DIVE_DELAY = 120;         // maximum number of seconds on the surface before the dive ends (less than this is considered the same dive)

// display settings
uint16_t Settings::TITLE_COLOR = CYAN;
uint16_t Settings::VALUE_COLOR = WHITE;
uint16_t Settings::ERROR_COLOR = RED;
uint16_t Settings::WARNING_COLOR = ORANGE;

void Settings::init(FileSystem *fileSystem) {
    Serial.println(F("Initializing settings."));
    fileSystem->loadSettings(this);
}

double Settings::getWaterPressure() { // bar/m
    // not using boolean for SALINITY on purpose (allows other options in the future (e.g. EN13319))
    if (SALINITY == SALT) {
        return Constants::SALT_WATER_PRESSURE;
    } else if (SALINITY == FRESH) {
        return Constants::FRESH_WATER_PRESSURE;
    }
    return Constants::SALT_WATER_PRESSURE; // default salt (most conservative)
}


JsonObject Settings::serializeObject(JsonObject &doc) {
    doc["gf_low"] = GF_LOW;
    doc["gf_high"] = GF_HIGH;
    doc["deco_step_size"] = DECO_STEP_SIZE;
    doc["last_stop"] = LAST_STOP;
    doc["salinity"] = SALINITY;
    doc["surface_pressure"] = SURFACE_PRESSURE;
    doc["max_ppo2"] = MAX_PPO2;
    doc["max_end"] = MAX_END;
    doc["o2_narcotic"] = O2_NARCOTIC;
    doc["step_interval"] = STEP_INTERVAL;
    doc["start_of_dive_pressure"] = START_OF_DIVE_PRESSURE;
    doc["end_of_dive_pressure"] = END_OF_DIVE_PRESSURE;
    doc["end_of_dive_delay"] = END_OF_DIVE_DELAY;
    doc["title_color"] = TITLE_COLOR;
    doc["error_color"] = ERROR_COLOR;
    doc["warning_color"] = WARNING_COLOR;

    Serial.println("Saving settings.");
    serializeJsonPretty(doc, Serial);
    Serial.println();

    return doc;
}

void Settings::deserializeObject(JsonObject &doc) {
    GF_LOW = doc["gf_low"];
    GF_HIGH = doc["gf_high"];
    DECO_STEP_SIZE = doc["deco_step_size"];
    LAST_STOP = doc["last_stop"];
    SALINITY = doc["salinity"];
    SURFACE_PRESSURE = doc["surface_pressure"];
    MAX_PPO2 = doc["max_ppo2"];
    MAX_END = doc["max_end"];
    O2_NARCOTIC = doc["o2_narcotic"];
    STEP_INTERVAL = doc["step_interval"];
    START_OF_DIVE_PRESSURE = doc["start_of_dive_pressure"];
    END_OF_DIVE_PRESSURE = doc["end_of_dive_pressure"];
    END_OF_DIVE_DELAY = doc["end_of_dive_delay"];
    TITLE_COLOR = doc["title_color"];
    ERROR_COLOR = doc["error_color"];
    WARNING_COLOR = doc["warning_color"];

    //logSettings();
}

size_t Settings::getJsonSize() {
    return JSON_OBJECT_SIZE(16);
}

void Settings::logSettings() {
    logSettings(&Serial);
}

void Settings::logSettings(Print *print) {
    print->println(F("\nSETTINGS"));
    print->println(F("======================================"));
    print->print(F("GF_LOW: \t\t\t\t"));
    print->println(GF_LOW);
    print->print(F("GF_HIGH: \t\t\t\t"));
    print->println(GF_HIGH);
    print->print(F("DECO_STEP_SIZE: \t\t"));
    print->println(DECO_STEP_SIZE);
    print->print(F("LAST_STOP: \t\t\t\t"));
    print->println(LAST_STOP);
    print->print(F("SALINITY: \t\t\t\t"));
    print->println(SALINITY);
    print->print(F("MAX_PPO2: \t\t\t\t"));
    print->println(MAX_PPO2);
    print->print(F("MAX_END: \t\t\t\t"));
    print->println(MAX_END);
    print->print(F("O2_NARCOTIC: \t\t\t"));
    print->println(O2_NARCOTIC);
    print->print(F("STEP_INTERVAL: \t\t\t"));
    print->println(STEP_INTERVAL);
    print->print(F("START_OF_DIVE_PRESSURE:"));
    print->println(START_OF_DIVE_PRESSURE);
    print->print(F("END_OF_DIVE_PRESSURE: \t"));
    print->println(END_OF_DIVE_PRESSURE);
    print->print(F("END_OF_DIVE_DELAY: \t\t"));
    print->println(END_OF_DIVE_DELAY);
    print->print(F("TITLE_COLOR: \t\t\t"));
    print->println(TITLE_COLOR);
    print->print(F("VALUE_COLOR: \t\t\t"));
    print->println(VALUE_COLOR);
    print->print(F("ERROR_COLOR: \t\t\t"));
    print->println(ERROR_COLOR);
    print->print(F("WARNING_COLOR: \t\t\t"));
    print->println(WARNING_COLOR);
    print->println(F("======================================"));
}









