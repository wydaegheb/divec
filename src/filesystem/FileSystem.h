#ifndef DIVEC_FILESYSTEM_H
#define DIVEC_FILESYSTEM_H

#define SPI_MISO 19
#define SPI_MOSI 23
#define SPI_SCK 18
#define SS_PIN TF_CS
#define MOSI_PIN SPI_MOSI
#define MISO_PIN SPI_MISO
#define SCK_PIN SPI_SCK

#include <SD.h>
#include <filesystem/JsonSerializable.h>


// SD card FAT filenames -> must use the 8.3 format
#define SETTINGS_FILE "SETTINGS.JSN"
#define GASSES_FILE "GASSES.JSN"
#define DECO_STATE_FILE "DECOST.JSN"
#define LOGBOOK_FILE "LOGBOOK.JSN"
#define TMP_LOG_FILE "TMPLOG.JSN"


class FileSystem {
public:

    FileSystem() = default;

    bool
    init(); // this code would normally go into the constructor but then Serial.begin() is not yet called -> program locks up when doing Serial.print in constructor

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

    // during the dive every step is written to a tmp file. Only if the dive finishes "normally" we rename this file to pr_<divenr>.jsn
    void saveDiveLogStep(JsonObject diveStep);

    void removeTmpDiveLog();

private:
    bool loadFromJsonFile(char const *fileName, JsonSerializable *jsonSerializable);

    bool saveToJsonFile(char const *fileName, JsonSerializable *jsonSerializable);

    bool saveToJsonFile(char const *fileName, JsonSerializable *jsonSerializable, bool append);

    File _diveLogFile;
};


#endif