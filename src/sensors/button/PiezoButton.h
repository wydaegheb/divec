#ifndef DIVEC_PIEZO_BUTTON_H
#define DIVEC_PIEZO_BUTTON_H

#define DEBOUNCE_TIME 500 // avoid more than 2 presses per second (debounce)

#include <Arduino.h>

class PiezoButton {
public:

    void init(uint8_t buttonPin, void (*interrupt)());

    bool isPressed();

    void onButtonClicked();

private:
    uint32_t _lastPressedReadingTimestamp;
    uint8_t _buttonPin;
    bool _pressed = false;

};

#endif