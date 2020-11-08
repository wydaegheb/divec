#include "Settings.h"

// buhlmann settings
double Settings::GF_LOW = 1.0;                      // gradient factor low
double Settings::GF_HIGH = 1.0;                     // gradient factor high
bool Settings::BUHLMANN_USE_1B = true;              // set to false to use tissue 1 iso 1b (not recommended)

// deco settings
uint8_t Settings::MIN_STOP_TIME = 60;               // minimum stop time intervals in seconds
uint8_t Settings::DECO_STEP_SIZE = 3;               // meters between decostops
uint8_t Settings::LAST_STOP = 6;                    // meters. meters below surface to make the last decostop
uint8_t Settings::SALINITY = SALT;                  // SALT/FRESH
double Settings::SURFACE_PRESSURE = 1.0;            // bar. default is pressure on the surface. should be measured en updated at start of dive
double Settings::MAX_PPO2 = 1.6;                    // bar.
uint16_t Settings::MAX_END = 30;                    // meters. max depth while using standard air (79% N2)
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

void Settings::init(FileSystem* fileSystem) {
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

size_t Settings::serialize(File* file) {
    DynamicJsonDocument doc(getFileSize());
    doc["GF_LOW"] = GF_LOW;
    doc["GF_HIGH"] = GF_HIGH;
    doc["BUHLMANN_USE_1B"] = BUHLMANN_USE_1B;
    doc["DECO_STEP_SIZE"] = DECO_STEP_SIZE;
    doc["LAST_STOP"] = LAST_STOP;
    doc["SALINITY"] = SALINITY;
    doc["SURFACE_PRESSURE"] = SURFACE_PRESSURE;
    doc["MAX_PPO2"] = MAX_PPO2;
    doc["MAX_END"] = MAX_END;
    doc["O2_NARCOTIC"] = O2_NARCOTIC;
    doc["STEP_INTERVAL"] = STEP_INTERVAL;
    doc["START_OF_DIVE_PRESSURE"] = START_OF_DIVE_PRESSURE;
    doc["END_OF_DIVE_PRESSURE"] = END_OF_DIVE_PRESSURE;
    doc["END_OF_DIVE_DELAY"] = END_OF_DIVE_DELAY;
    doc["TITLE_COLOR"] = TITLE_COLOR;
    doc["ERROR_COLOR"] = ERROR_COLOR;
    doc["WARNING_COLOR"] = WARNING_COLOR;
    Serial.println("Saving settings.");
    serializeJsonPretty(doc,Serial);
    Serial.println();
    return serializeJsonPretty(doc, *file);
}

DeserializationError Settings::deserialize(File* file) {
    DynamicJsonDocument doc(getFileSize());

    DeserializationError error = deserializeJson(doc, *file);
    if (error){ // stop deserializing if json parse failed
        return error;
    }

    GF_LOW = doc["GF_LOW"];
    GF_HIGH = doc["GF_HIGH"];
    BUHLMANN_USE_1B = doc["BUHLMANN_USE_1B"];
    DECO_STEP_SIZE = doc["DECO_STEP_SIZE"];
    LAST_STOP = doc["LAST_STOP"];
    SALINITY = doc["SALINITY"];
    SURFACE_PRESSURE = doc["SURFACE_PRESSURE"];
    MAX_PPO2 = doc["MAX_PPO2"];
    MAX_END = doc["MAX_END"];
    O2_NARCOTIC = doc["O2_NARCOTIC"];
    STEP_INTERVAL = doc["STEP_INTERVAL"];
    START_OF_DIVE_PRESSURE = doc["START_OF_DIVE_PRESSURE"];
    END_OF_DIVE_PRESSURE = doc["END_OF_DIVE_PRESSURE"];
    END_OF_DIVE_DELAY = doc["END_OF_DIVE_DELAY"];
    TITLE_COLOR = doc["TITLE_COLOR"];
    ERROR_COLOR = doc["ERROR_COLOR"];
    WARNING_COLOR = doc["WARNING_COLOR"];

    //logSettings();
    return error;
}

void Settings::logSettings() {
    Serial.println(F("\nSETTINGS"));
    Serial.println(F("======================================"));
    Serial.print(F("GF_LOW: \t\t\t\t"));
    Serial.println(GF_LOW);
    Serial.print(F("GF_HIGH: \t\t\t\t"));
    Serial.println(GF_HIGH);
    Serial.print(F("BUHLMANN_USE_1B: \t\t"));
    Serial.println(BUHLMANN_USE_1B);
    Serial.print(F("DECO_STEP_SIZE: \t\t"));
    Serial.println(DECO_STEP_SIZE);
    Serial.print(F("LAST_STOP: \t\t\t\t"));
    Serial.println(LAST_STOP);
    Serial.print(F("SALINITY: \t\t\t\t"));
    Serial.println(SALINITY);
    Serial.print(F("MAX_PPO2: \t\t\t\t"));
    Serial.println(MAX_PPO2);
    Serial.print(F("MAX_END: \t\t\t\t"));
    Serial.println(MAX_END);
    Serial.print(F("O2_NARCOTIC: \t\t\t"));
    Serial.println(O2_NARCOTIC);
    Serial.print(F("STEP_INTERVAL: \t\t\t"));
    Serial.println(STEP_INTERVAL);
    Serial.print(F("START_OF_DIVE_PRESSURE:"));
    Serial.println(START_OF_DIVE_PRESSURE);
    Serial.print(F("END_OF_DIVE_PRESSURE: \t"));
    Serial.println(END_OF_DIVE_PRESSURE);
    Serial.print(F("END_OF_DIVE_DELAY: \t\t"));
    Serial.println(END_OF_DIVE_DELAY);
    Serial.print(F("TITLE_COLOR: \t\t\t"));
    Serial.println(TITLE_COLOR);
    Serial.print(F("VALUE_COLOR: \t\t\t"));
    Serial.println(VALUE_COLOR);
    Serial.print(F("ERROR_COLOR: \t\t\t"));
    Serial.println(ERROR_COLOR);
    Serial.print(F("WARNING_COLOR: \t\t\t"));
    Serial.println(WARNING_COLOR);
    Serial.println(F("======================================"));
}

size_t Settings::getFileSize() {
    return JSON_OBJECT_SIZE(17) + 250; // 250 extra needed for string duplication (see https://arduinojson.org/v6/assistant/)
}






