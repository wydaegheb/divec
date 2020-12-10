#include "Arduino.h"
#include "DiveController.h"

DiveController diveController;

void setup() {
    Serial.begin(115200);
    unsigned long t = millis();
    while (!Serial.available() && ((millis() - t) < 5000)); // wait max 5 seconds
    delay(5000);

    diveController.setup();

/*    // test - setup
    DecoManager decoManager = diveController.getDecoManager();
    Serial.print(F("Using algorithm: "));
    Serial.println(decoManager.getCurrentAlgorithm()->getName());

    // mark every gas that's not air as not active
    for (auto gas: decoManager.getGasManager()->getOcGasses()) {
        gas->setActive(strcmp(gas->getName(), "AIR") == 0);
    }
    Settings::GF_HIGH = 0.85;
    Settings::GF_LOW = 0.30;
    Settings::MIN_STOP_TIME = 1;
    Settings::O2_NARCOTIC = false;


    // test decoplan
    uint32_t time = Time::getTime();
    Serial.print(F("Start time - "));
    Serial.println(DateTime(time).timestamp());

    decoManager.getCurrentDive()->start(time);

    Serial.println(F("\nsensor update - 4.0 bar (30 m) - 180 sec (3 min) (= descent to 30 meter)"));
    time += 180;
    decoManager.update(time, 4.0, 20.0, true);

    Serial.println(F("\nsensor update - 4.0 bar (30 m) - 1200 sec (16 min) (= stay 20 min 30 meter)"));
    time += 960;
    decoManager.update(time, 4.0, 20.0, true);

    Serial.println(F("\nsensor update - 2.0 bar (10 m) - 180 sec (3 min) (= ascent to 10 meter)"));
    time += 180;
    decoManager.update(time, 2.0, 20.0, true);

    Serial.println(F("\nsensor update - 1.0 bar (0 m) - 180 sec (3 min) (= ascent to surface)"));
    time += 180;
    decoManager.update(time, 1.0, 20.0, true);

    Serial.println(F("\nsensor update - 1.0 bar (0 m) - 180 sec (3 min) (= stay on surface for 3 min)"));
    time += 180;
    decoManager.update(time, 1.0, 20.0, true);

    decoManager.getDecoPlan()->log(decoManager.getCurrentDive());
    decoManager.getNdlInSeconds();


    Serial.println(F("\nre-loading dive"));
    uint16_t diveNr = decoManager.getLogBook()->getNumberOfDives();
    decoManager.loadDive(diveNr)->log();*/

}

void loop() {
    diveController.step();
}







