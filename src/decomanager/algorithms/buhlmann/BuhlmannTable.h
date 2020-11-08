#ifndef DIVEC_BUHLMANN_TABLE_H
#define DIVEC_BUHLMANN_TABLE_H

#include <domain/settings/Settings.h>

class BuhlmannTable {
public:

    explicit BuhlmannTable(double table[16][6]);

    double getN2HalfLife(uint8_t tissueNr);

    double getHeHalfLife(uint8_t tissueNr);

    double getN2A(uint8_t tissueNr);

    double getN2B(uint8_t tissueNr);

    double getHeA(uint8_t tissueNr);

    double getHeB(uint8_t tissueNr);

private:
    double _n2A[16];

    double _n2B[16];

    double _heA[16];

    double _heB[16];

    double _n2HalfLife[16];

    double _heHalfLife[16];
};

#endif