#ifndef DIVEC_FILESYSTEM_H
#define DIVEC_FILESYSTEM_H

#include <SdFat.h>
#include <filesystem/JsonSerializable.h>

#define SD_CS  5 // SD card select pin

// SD card FAT filenames -> must use the 8.3 format
#define SETTINGS_FILE "settings.jsn"
#define GASSES_FILE "gasses.jsn"
#define DECO_STATE_FILE "decost.jsn"
#define LOGBOOK_FILE "logbook.jsn"
#define TMP_LOG_FILE "tmplog.jsn"



class FileSystem {
public:

    void init();

    void loadSettings(JsonSerializable *settings);

    void loadGasses(JsonSerializable *gasManager);

    void loadDecoState(JsonSerializable *decoManager);

    void saveSettings(JsonSerializable *settings);

    void saveGasses(JsonSerializable *gasManager);

    void saveDecoState(JsonSerializable *decoManager);

    void loadLogbook(JsonSerializable *logbook);

    void saveLogbook(JsonSerializable *logbook);

    void loadDiveLog(JsonSerializable *dive, uint16_t diveNr);

    void saveDiveLog(JsonSerializable *dive, uint16_t diveNr);

    // during the dive every step is written to a tmp file. if the dive finishes "normally" we write a proper dive log and delete this file
    void writeTmpDiveLogStep(JsonSerializable *diveLogStep);

    void clearTmpDiveLog();

private:
    bool loadFromJsonFile(char const *fileName, JsonSerializable *jsonSerializable);

    bool saveToJsonFile(char const *fileName, JsonSerializable *jsonSerializable);

    bool saveToJsonFile(char const *fileName, JsonSerializable *jsonSerializable, bool append);

    File _diveLogFile;
    SdFat _sdFat;
};


#endif