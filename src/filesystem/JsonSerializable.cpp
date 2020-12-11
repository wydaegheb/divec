#include "JsonSerializable.h"

size_t JsonSerializable::save(File *file) {
    DynamicJsonDocument doc(getJsonSize() + BUFFER_FOR_STRINGS_DUPLICATION);
    JsonObject docObject = doc.to<JsonObject>();
    return serializeJson(serializeObject(docObject), *file);
}

DeserializationError JsonSerializable::load(File *file) {
    DynamicJsonDocument doc(getJsonSize() + BUFFER_FOR_STRINGS_DUPLICATION);

    DeserializationError error = deserializeJson(doc, *file);
    if (error) { // stop deserializing if json parse failed
        return error;
    }

    JsonObject docObject = doc.as<JsonObject>();
    deserializeObject(docObject);

    return error;
}
