#include "PiezoButton.h"

void PiezoButton::init(uint8_t buttonPin, void (*interrupt)()) {
    Serial.print(F("Initializing piezo button on pin: "));
    Serial.println(buttonPin);
    _buttonPin = buttonPin;
    pinMode(buttonPin, INPUT_PULLDOWN);
    attachInterrupt(buttonPin, interrupt, CHANGE);
    Serial.println(F(" - piezo button initialized."));
}

bool PiezoButton::isPressed() {
    if (_pressed) {
        Serial.print(F("Piezo button isPressed on pin: "));
        Serial.println(_buttonPin);
        _pressed = false;
        return true;
    }
    return false;
}

void PiezoButton::onButtonClicked() {
    Serial.print(F("Piezo button onButtonClicked on pin: "));
    Serial.println(_buttonPin);
    _pressed = true;
}
