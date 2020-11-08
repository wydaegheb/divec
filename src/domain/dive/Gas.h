#ifndef DIVEC_GAS_H
#define DIVEC_GAS_H

#include <decomanager/algorithms/equations/DiveEquations.h>


class Gas {
public:
    Gas(double o2,char const* name);

    Gas(double o2, double he,char const* name);

    Gas(double o2, double he,char const* name, bool active);

    bool isActive() const;

    void setActive(bool active);

    char const* getName();

    double getO2();

    void setO2(double o2);

    double getN2();

    void setN2(double n2);

    double getHe();

    void setHe(double he);

    bool isUsable(double pressureInBars);

    uint16_t calcMODInMeters();

    uint16_t calcENDInMeters(double pressureInBars);


private:

    void resolveName();

    bool _active;

    char const* _name;
    char _mix[8];

    // stored as fractions (e.g. 20% o2 -> _o2 = 0.2). Total must be 1.
    double _o2;
    double _n2;
    double _he;
};

#endif