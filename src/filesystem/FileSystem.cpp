#include "FileSystem.h"

void FileSystem::init() {
    // SD Card initialization
    Serial.println(F("Initializing file system."));
    if (_sdFat.begin(SD_CS, SPI_HALF_SPEED)) {
        Serial.println(F(" - file system initialized."));
    } else {
        Serial.println(F("!!! SYSTEM ERROR !!!\n[Filesystem failed]"));
        exit(0);
    }
}

void FileSystem::loadSettings(JsonSerializable *settings) {
    if (!loadFromJsonFile(SETTINGS_FILE, settings)) {
        saveSettings(settings);
    }
}

void FileSystem::saveSettings(JsonSerializable *settings) {
    saveToJsonFile(SETTINGS_FILE, settings);
}

void FileSystem::loadGasses(JsonSerializable *gasManager) {
    if (!loadFromJsonFile(GASSES_FILE, gasManager)) {
        saveGasses(gasManager);
    }
}

void FileSystem::saveGasses(JsonSerializable *gasManager) {
    saveToJsonFile(GASSES_FILE, gasManager);
}

void FileSystem::loadDecoState(JsonSerializable *decoManager) {
    if (!loadFromJsonFile(DECO_STATE_FILE, decoManager)) {
        saveDecoState(decoManager);
    }
}

void FileSystem::saveDecoState(JsonSerializable *decoManager) {
    saveToJsonFile(DECO_STATE_FILE, decoManager);
}

void FileSystem::loadLogbook(JsonSerializable *logbook) {
    if (!loadFromJsonFile(LOGBOOK_FILE, logbook)) {
        saveLogbook(logbook);
    }
}

void FileSystem::saveLogbook(JsonSerializable *logbook) {
    saveToJsonFile(LOGBOOK_FILE, logbook);
}

void FileSystem::loadDiveLog(JsonSerializable *dive, uint16_t diveNr) {
    char fileName[100];
    snprintf(fileName, 100, "dv_%d.jsn", diveNr);
    loadFromJsonFile(fileName, dive);
}


void FileSystem::saveDiveLog(JsonSerializable *dive, uint16_t diveNr) {
    char fileName[100];
    snprintf(fileName, 100, "dv_%d.jsn", diveNr);
    if (_diveLogFile) { // if a log exists -> close it first.
        Serial.println(F("This should not be possible! Log for this dive already exists! - Removing previous file."));
        _diveLogFile.remove();
    }
    saveToJsonFile(fileName, dive);
}

void FileSystem::writeTmpDiveLogStep(JsonSerializable *diveLogStep) {
    saveToJsonFile(TMP_LOG_FILE, diveLogStep, true);
}

void FileSystem::clearTmpDiveLog() {
    _sdFat.remove(TMP_LOG_FILE);
}


bool FileSystem::loadFromJsonFile(char const *fileName, JsonSerializable *jsonSerializable) {
    bool result = false;
    Serial.print(F(" -> loading file:"));
    Serial.print(fileName);
    Serial.print(F(" - "));

    // open the file for reading
    File file = _sdFat.open(fileName);

    // if the file opened okay, load from it:
    if (file && file.size() > 10) {
        // Deserialize the JSON document
        DeserializationError error = jsonSerializable->load(&file);
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

bool FileSystem::saveToJsonFile(char const *fileName, JsonSerializable *jsonSerializable) {
    return saveToJsonFile(fileName, jsonSerializable, false);
}

bool FileSystem::saveToJsonFile(const char *fileName, JsonSerializable *jsonSerializable, bool append) {
    bool result = false;
    Serial.print(F(" - saving file:"));
    Serial.print(fileName);
    Serial.print(F(" - "));


    if (!append) {
        // Not appending. deleting existing file (if it exists)
        _sdFat.remove(fileName);
    }

    // Open file for writing
    File file = _sdFat.open(fileName, FILE_WRITE);

    // if the filesystem opened okay, write to it:
    if (file) {
        // Serialize JSON to file
        if (jsonSerializable->save(&file) == 0) {
            Serial.println(F("FAILED (0 bytes written)."));
        } else {
            if (append) {
                file.write("\n");
            }
            Serial.println(F("Success"));
            result = true;
        }
        file.close();
    } else {
        Serial.println(F("FAILED."));
    }
    return result;
}











