#ifndef DIVEC_JSONSERIALIZABLE_H
#define DIVEC_JSONSERIALIZABLE_H

#include <ArduinoJson.h>

class JsonSerializable{
public:
    virtual size_t serialize(File* file) = 0;
    virtual DeserializationError deserialize(File* file) = 0;
    virtual size_t getFileSize() = 0;
};

#endif //DIVEC_JSONSERIALIZABLE_H
