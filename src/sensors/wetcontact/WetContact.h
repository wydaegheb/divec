#ifndef DIVEC_WETCONTACT_H
#define DIVEC_WETCONTACT_H

#include <Arduino.h>

#define WET_CONTACT_ACTIVATION_TRESHOLD 15000 // WETCONTACT IS CONSIDERED "ACTIVE" AFTER 15 seconds in the water

class WetContact {
public:

    WetContact() = default;

    void init(uint8_t wetContactPin, void (*interrupt)()); // this code would normally go into the constructor but then Serial.begin() is not yet called -> program locks up when doing Serial.print in constructor

    bool isActivated() const;

    void onChange();

private:
    uint8_t _wetContactPin;
    volatile uint32_t _lastActive; // used in interrupt -> mark as volatile
    volatile bool _activated;      // used in interrupt -> mark as volatile
    volatile int _previousValue;   // used in interrupt -> mark as volatile
};


#endif //DIVEC_WETCONTACT_H
