#include "BuhlmannTable.h"

BuhlmannTable::BuhlmannTable(double table[17][6]) {
    for (uint8_t tissueNr = 0; tissueNr < 16; tissueNr++) {
        _n2HalfLife[tissueNr] = table[tissueNr + 1][0];
        _n2A[tissueNr] = table[tissueNr + 1][1];
        _n2B[tissueNr] = table[tissueNr + 1][2];

        _heHalfLife[tissueNr] = table[tissueNr + 1][3];
        _heA[tissueNr] = table[tissueNr + 1][4];
        _heB[tissueNr] = table[tissueNr + 1][5];
    }
}

double BuhlmannTable::getN2HalfLife(uint8_t tissueNr) {
    return _n2HalfLife[tissueNr];
}

double BuhlmannTable::getHeHalfLife(uint8_t tissueNr) {
    return _heHalfLife[tissueNr];
}

double BuhlmannTable::getN2A(uint8_t tissueNr) {
    return _n2A[tissueNr];
}

double BuhlmannTable::getN2B(uint8_t tissueNr) {
    return _n2B[tissueNr];
}

double BuhlmannTable::getHeA(uint8_t tissueNr) {
    return _heA[tissueNr];
}

double BuhlmannTable::getHeB(uint8_t tissueNr) {
    return _heB[tissueNr];
}

