#include "FileSystem.h"

void FileSystem::init() {
    Serial.println(F("Initializing file system."));
    if (_sdFat.begin(SD_CS, SD_SCK_MHZ(10))) { // Breakouts require 10 MHz limit due to longer wires
        _reader = new Adafruit_ImageReader(_sdFat);
        Serial.println(F(" - file system initialized."));
    } else {
        Serial.println(F("!!! SYSTEM ERROR !!!\n[Filesystem failed]"));
        exit(0);
    }
}

uint16_t *FileSystem::loadBgImage() {
    Adafruit_Image _backgroundImage;
    _reader->loadBMP((char *) BG_IMAGE_FILE, _backgroundImage);
    return ((GFXcanvas16 *) _backgroundImage.getCanvas())->getBuffer();
}

void FileSystem::loadSettings(JsonSerializable* settings) {
    if (!loadFromJsonFile(SETTINGS_FILE,settings)){
        saveSettings(settings);
    }
}

void FileSystem::saveSettings(JsonSerializable* settings) {
    saveToJsonFile(SETTINGS_FILE, settings);
}

void FileSystem::loadGasses(JsonSerializable* gasManager) {
    if (!loadFromJsonFile(GASSES_FILE,gasManager)){
        saveGasses(gasManager);
    }
}

void FileSystem::saveGasses(JsonSerializable* gasManager) {
    saveToJsonFile(GASSES_FILE, gasManager);
}

void FileSystem::loadDecoState(JsonSerializable* decoManager) {
    if (!loadFromJsonFile(DECO_STATE_FILE,decoManager)){
        saveDecoState(decoManager);
    }
}

void FileSystem::saveDecoState(JsonSerializable* decoManager) {
    saveToJsonFile(DECO_STATE_FILE, decoManager);
}

bool FileSystem::loadFromJsonFile(char const* fileName, JsonSerializable* jsonSerializable) {
    bool result = false;
    Serial.print(F(" -> loading file:"));
    Serial.print(fileName);
    Serial.print(F(" - "));

    // open the file for reading
    File file = _sdFat.open(fileName);

    // if the file opened okay, load from it:
    if (file && file.size() > 10) {
        // Deserialize the JSON document
        DeserializationError error = jsonSerializable->deserialize(&file);
        file.close();
        if (error) {
            Serial.print(F("FAILED. error:"));
            Serial.println(error.c_str());
        } else {
            Serial.println(F("Success"));
            result = true;
        }
    } else {
        Serial.println(F("FAILED to open."));
    }
    return result;
}

bool FileSystem::saveToJsonFile(char const* fileName, JsonSerializable* jsonSerializable) {
    bool result = false;
    Serial.print(F(" - saving file:"));
    Serial.print(fileName);
    Serial.print(F(" - "));
    // Delete existing file, otherwise the configuration is appended to the file
    _sdFat.remove(fileName);

    // Open file for writing
    File file = _sdFat.open(fileName, FILE_WRITE);

    // if the filesystem opened okay, write to it:
    if (file) {
        // Serialize JSON to file
        if (jsonSerializable->serialize(&file) == 0) {
            Serial.println(F("FAILED."));
        } else {
            Serial.println(F("Success"));
            result = true;
        }
        file.close();
    } else {
        Serial.println(F("FAILED."));
    }
    return result;
}





