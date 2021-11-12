#include "FileSystem.h"


bool FileSystem::init() {
    // SD Card initialization
    Serial.println(F("Initializing file system."));
    if (SD.begin(TF_CS)) {
        Serial.println(F(" - file system initialized."));
    } else {
        Serial.println(F("!!! SYSTEM ERROR !!!\n[Filesystem failed]"));
        return false;
    }
    return true;
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
    if (_diveLogFile) { // if a log exists -> remove it first.
        Serial.println(F("This should not be possible! Log for this dive already exists! - Removing previous file."));
        SD.remove(fileName);
    }
    saveToJsonFile(fileName, dive);
    snprintf(fileName, 100, "pr_%d.jsn", diveNr);

    size_t n;
    uint8_t buf[64];
    File tmpFile = SD.open(TMP_LOG_FILE);
    File finalLogFile = SD.open(fileName);
    if (!tmpFile && !finalLogFile) {
        Serial.println(F("Rename tmp log dive steps failed!"));
    } else {
        while ((n = tmpFile.read(buf, sizeof(buf))) > 0) {
            finalLogFile.write(buf, n);
        }
    }
    tmpFile.close();
    finalLogFile.close();
}

void FileSystem::saveDiveLogStep(JsonObject diveStep) {
    Serial.print(F(" - saving dive step to:"));
    Serial.print(TMP_LOG_FILE);
    Serial.print(F(" - "));


    // Open file for writing
    File file = SD.open(TMP_LOG_FILE, FILE_WRITE);

    // if the filesystem opened okay, write to it:
    if (file) {
        // Serialize JSON to file
        if (serializeJson(diveStep, file) == 0) {
            Serial.println(F("FAILED (0 bytes written)."));
        } else {
            file.write("\n");
            Serial.println(F("Success"));
        }
        file.close();
    } else {
        Serial.println(F("FAILED."));
    }
}

void FileSystem::removeTmpDiveLog() {
    SD.remove(TMP_LOG_FILE);
}


bool FileSystem::loadFromJsonFile(char const *fileName, JsonSerializable *jsonSerializable) {
    bool result = false;
    Serial.print(F(" -> loading file:"));
    Serial.print(fileName);
    Serial.print(F(" - "));

    // open the file for reading
    File file = SD.open(fileName);

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

    // Open file for writing
    File file = SD.open(fileName, FILE_WRITE);

    // if the filesystem opened okay, write to it:
    if (file) {
        // Serialize JSON to file
        if (jsonSerializable->save(&file) == 0) {
            Serial.println(F("FAILED (0 bytes written)."));
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













