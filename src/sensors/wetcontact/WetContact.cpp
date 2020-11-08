#include "WetContact.h"


void WetContact::init(uint8_t wetContactPin, void (*interrupt)()) {
    Serial.println(F("Initializing wet contact."));
    _activated = false; // Turned OFF
    _lastActive = 0;
    _previousValue = HIGH;
    _wetContactPin = wetContactPin;
    pinMode(wetContactPin, INPUT_PULLDOWN);
    attachInterrupt(wetContactPin, interrupt, CHANGE);
    Serial.println(F(" - wet contact initialized."));
}

bool WetContact::isActivated() const {
    return (_activated && ((millis() - _lastActive) > WET_CONTACT_ACTIVATION_TRESHOLD));
}

void WetContact::onChange() {
    int currentValue = digitalRead(_wetContactPin);

    if (_previousValue == HIGH && currentValue == LOW) {
        Serial.println(F("WATER SWITCH - ON"));
        _activated = true; // Turned ON
        _lastActive = millis();
    } else if (_previousValue == LOW && currentValue == HIGH) {
        Serial.println(F("WATER SWITCH - OFF"));
        _activated = false; // Turned OFF
        _lastActive = 0;
    }
    _previousValue = currentValue;
}