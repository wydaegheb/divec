#ifndef DIVEC_PIEZO_BUTTON_H
#define DIVEC_PIEZO_BUTTON_H

#include "Arduino.h"

class PiezoButton {
public:

    void init(uint8_t buttonPin, void (*interrupt)());

    bool isPressed();

    void onButtonClicked();


private:
    uint8_t _buttonPin;
    bool _pressed = false;

};

#endif