#include "Bluetooth.h"

Bluetooth::~Bluetooth() {
    _bleSpiFriend->end();
    delete _bleSpiFriend;
}

bool Bluetooth::init() {
    /* Initialise the module */
    Serial.print(F("Initialising the Bluefruit LE module: "));

    // using software spi as hardwar is allready taken by the screen
    _bleSpiFriend = new Adafruit_BluefruitLE_SPI(BLUEFRUIT_SPI_SCK, BLUEFRUIT_SPI_MISO,
                                                 BLUEFRUIT_SPI_MOSI, BLUEFRUIT_SPI_CS, BLUEFRUIT_SPI_IRQ, BLUEFRUIT_SPI_RST);

    if (!_bleSpiFriend->begin(VERBOSE_MODE)) {
        Serial.println(F(" - ERROR: Couldn't find Bluefruit LE  Module!"));
        return false;
    }
    Serial.println(F(" - bluetooth module started"));

    /* Disable command echo from Bluefruit */
    _bleSpiFriend->echo(false);

    // Set Bluefruit to DATA mode
    _bleSpiFriend->setMode(BLUEFRUIT_MODE_DATA);
    Serial.println(F(" - bluetooth module switched to DATA mode."));

    Serial.println(F(" - bluetooth module initialized."));
}

void Bluetooth::receive() {
    /*   Serial.print(F(" - bluetooth module receive."));
       // Echo received data
       if (_bleSpiFriend->isConnected()) {
           Serial.print(F(" - connected. read: "));
           int command = _bleSpiFriend->read();
           Serial.println(command);

           switch (command) {
               case 'V': {   // Get Version
                   Serial.println(F("V"));
                   _bleSpiFriend->info(); // Print Bluefruit information
                   break;
               }

               case 'S': {   // Setup dimensions, components, stride...
                   Serial.println(F("S"));
                   break;
               }

               case 'C': {   // Clear with color
                   Serial.println(F("C"));
                   break;
               }

               case 'B': {   // Set Brightness
                   Serial.println(F("B"));
                   break;
               }

               case 'P': {   // Set Pixel
                   Serial.println(F("P"));
                   break;
               }

               case 'I': {   // Receive new image
                   Serial.println(F("I"));
                   break;
               }

               default: {
                   Serial.print(F("Command:"));
                   Serial.print(command);
                   Serial.println(F(" ????????"));
               }

           }
       } else {
           Serial.println(F(" - not connected."));
       }
   */
}
