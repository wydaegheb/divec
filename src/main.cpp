#include "Arduino.h"
#include "DiveController.h"

DiveController diveController;

void setup() {
    Serial.begin(115200);
    unsigned long t = millis();
    while (!Serial.available() && ((millis() - t) < 5000)); // wait max 5 seconds
    delay(5000);

    diveController.setup();


/*
    // test - setup
    DecoManager decoManager = diveController.getDecoManager();
    Serial.print(F("Using algorithm: "));
    Serial.println(decoManager.getCurrentAlgorithm()->getName());

    // mark every gas that's not air as not active
    for (auto gas: decoManager.getGasManager()->getOcGasses()) {
        gas->setActive(strcmp(gas->getName(), "AIR") == 0);
    }
    Settings::GF_HIGH=0.85;
    Settings::GF_LOW=0.35;
    Settings::MIN_STOP_TIME=1;
    Settings::O2_NARCOTIC=false;


    // test decoplan
    DateTime time = Time::getTime();
    decoManager.getCurrentDive()->start(time);

    Serial.println(F("\nsensor update - 4.0 bar (30 m) - 180 sec (3 min) (= descent to 30 meter)"));
    time = time + TimeSpan(180);
    decoManager.update(time, 4.0, 20.0, true);
    decoManager.getDecoPlan()->log(decoManager.getCurrentDive());

    Serial.println(F("\nsensor update - 4.0 bar (30 m) - 2820 sec (47 min) (= stay 47 minutes on 30 meter)"));
    time = time + TimeSpan(3000);
    decoManager.update(time, 4.0, 20.0, true);
    decoManager.getDecoPlan()->log(decoManager.getCurrentDive());
*/


}

/*bool pressed;
uint32_t prevPiezoValue = 0;
uint32_t minV = 100000;
uint32_t maxV = 0;*/

void loop() {
    diveController.step();

/*    uint32_t piezoValue = analogRead(RIGHT_BUTTON_PIN);
    if (piezoValue < minV) {
        minV = piezoValue;
        Serial.print(F("MIN > "));
        Serial.println(piezoValue);
    }
    if (piezoValue > maxV) {
        maxV = piezoValue;
        Serial.print(F("MAX > "));
        Serial.println(piezoValue);
    }

    if (piezoValue > 300) {
        Serial.print(F("P> "));
        Serial.println(piezoValue);
    }*/

    //diveController.step();
}







