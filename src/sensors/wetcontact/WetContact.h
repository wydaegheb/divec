#ifndef DIVEC_WETCONTACT_H
#define DIVEC_WETCONTACT_H

#include "Arduino.h"

#define WET_CONTACT_ACTIVATION_TRESHOLD 15000 // WETCONTACT IS CONSIDERED "ACTIVE" AFTER 15 seconds in the water

class WetContact {
public:

    void init(uint8_t wetContactPin, void (*interrupt)());

    bool isActivated() const;

    void onChange();


private:
    uint8_t _wetContactPin;
    uint32_t _lastActive;
    bool _activated;
    int _previousValue;
};


#endif //DIVEC_WETCONTACT_H
