#include "Bluetooth.h"

bool Bluetooth::init() {
    /* Initialise the module */
    Serial.print(F("Initialising the Bluefruit LE module: "));

    // using software spi as hardware is already taken by the screen
    _bleSpiFriend = new Adafruit_BluefruitLE_SPI(BLUEFRUIT_SPI_SCK, BLUEFRUIT_SPI_MISO,
                                                 BLUEFRUIT_SPI_MOSI, BLUEFRUIT_SPI_CS, BLUEFRUIT_SPI_IRQ, BLUEFRUIT_SPI_RST);

    if (!_bleSpiFriend->begin(VERBOSE_MODE)) {
        Serial.println(F(" - ERROR: Couldn't find Bluefruit LE  Module!"));
        return false;
    }
    Serial.println(F(" - bluetooth module started"));

    /* Disable command echo from Bluefruit */
    _bleSpiFriend->echo(false);

    // Setting device name to 'DiveC'
    if (!_bleSpiFriend->sendCommandCheckOK(F("AT+GAPDEVNAME=DiveC"))) {
        Serial.println(F("ERROR: Could not set device name"));
    }

    // Set Bluefruit to DATA mode
    _bleSpiFriend->setMode(BLUEFRUIT_MODE_DATA);
    Serial.println(F(" - bluetooth module switched to DATA mode."));

    Serial.println(F(" - bluetooth module initialized."));
    return true;

}

int Bluetooth::receive() const {
    if (_bleSpiFriend->available()) {
        return _bleSpiFriend->read();
    }
    return -1;
}
