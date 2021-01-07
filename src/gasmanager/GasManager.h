#ifndef DIVEC_GASMANAGER_H
#define DIVEC_GASMANAGER_H


#include <domain/dive/Gas.h>

#define MAX_GASSES 30 // not a real limit - used to allocate json documents - increase if more are needed

class GasManager : public JsonSerializable {
public:
    // default gasses
    static Gas AIR;
    static Gas NX32;
    static Gas NX36;
    static Gas NX40;
    static Gas NX50;
    static Gas TX18_35;

    void clear();

    void init(FileSystem *fileSystem);

    void addGas(Gas *divegas);

    Gas *getCurrentGas();

    Gas *setCurrentGas(Gas *currentOcGas);

    Gas *getBestGas(uint16_t depthInMeters);

    Gas *getGas(char const *name);

    Gas *getGas(int index);

    uint8_t getNrOfGasses() const;


    JsonObject serializeObject(JsonObject &doc) final;

    void deserializeObject(JsonObject &doc) final;

    size_t getJsonSize() final;

private:
    Gas *_currentGas;

    Gas *_gasses[MAX_GASSES];

    uint8_t _nrOfGasses;

private:

    void loadDefaultGasses();

};


#endif //DIVEC_GASMANAGER_H
