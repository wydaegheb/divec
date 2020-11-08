#include "PiezoButton.h"

void PiezoButton::init(int buttonPin, void (*interrupt)()) {
    Serial.print(F("Initializing piezo button on pin: "));
    Serial.println(buttonPin);
    pinMode(buttonPin, INPUT_PULLDOWN);
    attachInterrupt(buttonPin, interrupt, CHANGE);
    Serial.println(F(" - piezo button initialized."));
}

bool PiezoButton::isPressed() {
    if (_pressed) {
        _pressed = false;
        return true;
    }
    return false;
}

void PiezoButton::onButtonClicked() {
    _lastPress = millis();
    _pressed = true;
}
