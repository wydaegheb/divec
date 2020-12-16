#include "Arduino.h"
#include "DiveController.h"

DiveController diveController;

void setup() {
    Serial.begin(115200);
    unsigned long t = millis();
    while (!Serial.available() && ((millis() - t) < 5000)); // wait max 5 seconds

    delay(5000); // a bit of extra time to open the monitor console

    diveController.setup();

}

void loop() {
    diveController.step();
}







