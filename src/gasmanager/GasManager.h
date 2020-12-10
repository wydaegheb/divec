#ifndef DIVEC_GASMANAGER_H
#define DIVEC_GASMANAGER_H


#include "Arduino.h"

#include <list>
#include <domain/dive/Gas.h>
#include <filesystem/FileSystem.h>

#define MAX_OC_GASSES 30 // not a real limit - used to allocate json documents - increase if more are needed
#define MAX_CC_GASSES 30 // not a real limit - used to allocate json documents - increase if more are needed

class GasManager : public JsonSerializable {
public:
    // default gasses
    static Gas AIR;
    static Gas NX32;
    static Gas NX36;
    static Gas NX40;
    static Gas NX50;
    static Gas TX18_35;

    ~GasManager() override = default;

    void init(FileSystem* fileSystem);

    void clear();


    void addOcGas(Gas *divegas);

    Gas *getCurrentOcGas();

    Gas *setCurrentOcGas(Gas *currentOcGas);

    Gas *getBestOcGas(uint16_t depthInMeters);

    Gas *getOcGas(char const* name);

    std::list<Gas *> getOcGasses();


    void addCcGas(Gas *divegas);

    Gas *getCurrentCcGas();

    Gas *setCurrentCcGas(Gas *currentOcGas);

    Gas *getBestCcGas(uint16_t depthInMeters);

    Gas *getCcGas(char const* name);

    std::list<Gas *> getCcGasses();



    JsonObject serializeObject(JsonObject &doc) override;

    void deserializeObject(JsonObject &doc) override;

    size_t getFileSize() override;

private:

    Gas *getGas(char const* name, std::list<Gas *> *gasList);

    Gas *getBestGas(double pressureInBars, std::list<Gas *> *gasList);

    void loadDefaultGasses();

    Gas *_currentOcGas;
    Gas *_currentCcGas;

    std::list<Gas *> _ocGasses;
    std::list<Gas *> _ccGasses;

};


#endif //DIVEC_GASMANAGER_H
