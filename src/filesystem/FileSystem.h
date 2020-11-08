#ifndef DIVEC_FILESYSTEM_H
#define DIVEC_FILESYSTEM_H

#define SD_CS  5 // SD card select pin

#define SETTINGS_FILE "settings.jsn" // SD card FAT filenames -> must use the 8.3 format
#define GASSES_FILE "gasses.jsn"
#define DECO_STATE_FILE "decost.jsn"
#define LOGBOOK_FILE "logbook.jsn"
#define BG_IMAGE_FILE "jellyfish.bmp"

#include <SdFat.h>
#include <Adafruit_ImageReader.h>
#include "JsonSerializable.h"


class FileSystem {
public:

    void init();

    void loadSettings(JsonSerializable* settings);

    void loadGasses(JsonSerializable* gasManager);

    void loadDecoState(JsonSerializable* decoManager);

    void saveSettings(JsonSerializable* settings);

    void saveGasses(JsonSerializable* gasManager);

    void saveDecoState(JsonSerializable* decoManager);

    uint16_t *loadBgImage();

private:
    bool loadFromJsonFile(char const* fileName, JsonSerializable* jsonSerializable);

    bool saveToJsonFile(char const* fileName, JsonSerializable* jsonSerializable);

    SdFat _sdFat;
    Adafruit_ImageReader *_reader;
};


#endif