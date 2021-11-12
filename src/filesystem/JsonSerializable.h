#ifndef DIVEC_JSONSERIALIZABLE_H
#define DIVEC_JSONSERIALIZABLE_H

#include <ArduinoJson.h>
#include <SD.h>
//#include <FatLib/ArduinoFiles.h>

#define BUFFER_FOR_STRINGS_DUPLICATION 500

class JsonSerializable {
public:

    size_t save(File *file);

    DeserializationError load(File *file);


    virtual JsonObject serializeObject(JsonObject &doc) = 0;

    virtual void deserializeObject(JsonObject &doc) = 0;

    virtual size_t getJsonSize() = 0;


};

#endif //DIVEC_JSONSERIALIZABLE_H
