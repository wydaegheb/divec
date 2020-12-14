#include <tests/TestDive.h>
#include "Arduino.h"
#include "DiveController.h"

DiveController diveController;

void setup() {
    Serial.begin(115200);
    unsigned long t = millis();
    while (!Serial.available() && ((millis() - t) < 5000)); // wait max 5 seconds

    delay(5000); // a bit of extra time to open the monitor console

    diveController.setup();

    Settings::MIN_STOP_TIME = 1;

    Serial.println(F("Dive to 30m - 16 min bottom"));
    DecoManager decoManager = diveController.getDecoManager();
    auto *d1 = new TestDive(&decoManager);
    d1->setGF(30, 85);
    d1->addGas(&GasManager::AIR);
    d1->addDiveLeg(30, 16);
    d1->getDecoPlan()->log(decoManager.getDive());

    Serial.println(F("Dive to 30m - 50 min bottom"));
    auto *d2 = new TestDive(&decoManager);
    d2->setGF(30, 85);
    d2->addGas(&GasManager::AIR);
    d2->addDiveLeg(30, 50);
    d2->getDecoPlan()->log(decoManager.getDive());


}

void loop() {
    //diveController.step();
}







