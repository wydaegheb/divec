#ifndef DIVEC_PIEZO_BUTTON_H
#define DIVEC_PIEZO_BUTTON_H

#include "Arduino.h"

class PiezoButton {
public:

    void init(int buttonPin, void (*interrupt)());

    bool isPressed();

    void onButtonClicked();


private:
    bool _pressed = false;
    uint32_t _lastPress = 0; // the time of the last button press

};

#endif