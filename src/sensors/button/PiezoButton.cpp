#include "PiezoButton.h"

void PiezoButton::init(uint8_t buttonPin, void (*interrupt)()) {
    Serial.print(F("Initializing piezo button on pin: "));
    Serial.print(buttonPin);
    Serial.print(F(" -> digital pin: "));
    Serial.println(digitalPinToInterrupt(buttonPin));
    _buttonPin = buttonPin;
    _lastPressedReadingTimestamp = millis();
    pinMode(buttonPin, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(buttonPin), interrupt, CHANGE);
    Serial.println(F(" - piezo button initialized."));
}

bool PiezoButton::isPressed() {
    if (millis() - _lastPressedReadingTimestamp < DEBOUNCE_TIME) {
        _pressed = false;
        return false;
    }
    if (_pressed) {
        Serial.print(F("Piezo button isPressed on pin: "));
        Serial.println(_buttonPin);
        _pressed = false;
        _lastPressedReadingTimestamp = millis();
        return true;
    }
    return false;
}

// NOTE: No Serial.print or calls to time (millis() etc.) in interrupt handlers !
void PiezoButton::onButtonClicked() {
    _pressed = true;
}
