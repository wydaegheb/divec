#include <Adafruit_NeoPixel.h>
#include "Arduino.h"
#include "DiveController.h"

#include "Adafruit_BluefruitLE_SPI.h"
#include "bluetooth/BluefruitConfig.h"


DiveController diveController;


#define FACTORYRESET_ENABLE     1
#define NEOPIXEL_VERSION_STRING "Neopixel v2.0"
#define PIN                     8   /* Pin used to drive the NeoPixels */

#define MAXCOMPONENTS  4
uint8_t *pixelBuffer = NULL;
uint8_t width = 0;
uint8_t height = 0;
uint8_t stride;
uint8_t componentsValue;
bool is400Hz;
uint8_t components = 3;     // only 3 and 4 are valid values


Adafruit_NeoPixel neopixel = Adafruit_NeoPixel();


Adafruit_BluefruitLE_SPI ble(BLUEFRUIT_SPI_CS, BLUEFRUIT_SPI_IRQ, BLUEFRUIT_SPI_RST);

// A small helper
void error(const __FlashStringHelper*err) {
    Serial.println(err);
    while (1);
}

void serial_printf(const char * format, ...) {
    char buffer [48];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    Serial.print(buffer);
}

void sendResponse(char const *response) {
    serial_printf("Send Response: %s\n", response);
    ble.write(response, strlen(response)*sizeof(char));
}

void swapBuffers()
{
    uint8_t *base_addr = pixelBuffer;
    int pixelIndex = 0;
    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++) {
            if (components == 3) {
                neopixel.setPixelColor(pixelIndex, neopixel.Color(*base_addr, *(base_addr+1), *(base_addr+2)));
            }
            else {
                neopixel.setPixelColor(pixelIndex, neopixel.Color(*base_addr, *(base_addr+1), *(base_addr+2), *(base_addr+3) ));
            }
            base_addr+=components;
            pixelIndex++;
        }
        pixelIndex += stride - width;   // Move pixelIndex to the next row (take into account the stride)
    }
    neopixel.show();

}

void commandVersion() {
    Serial.println(F("Command: Version check"));
    sendResponse(NEOPIXEL_VERSION_STRING);
}

void commandSetup() {
    Serial.println(F("Command: Setup"));

    width = ble.read();
    height = ble.read();
    stride = ble.read();
    componentsValue = ble.read();
    is400Hz = ble.read();

    neoPixelType pixelType;
    pixelType = componentsValue + (is400Hz ? NEO_KHZ400 : NEO_KHZ800);

    components = (componentsValue == NEO_RGB || componentsValue == NEO_RBG || componentsValue == NEO_GRB || componentsValue == NEO_GBR || componentsValue == NEO_BRG || componentsValue == NEO_BGR) ? 3:4;

    serial_printf("\tsize: %dx%d\n", width, height);
    serial_printf("\tstride: %d\n", stride);
    serial_printf("\tpixelType %d\n", pixelType);
    serial_printf("\tcomponents: %d\n", components);

    if (pixelBuffer != NULL) {
        delete[] pixelBuffer;
    }

    uint32_t size = width*height;
    pixelBuffer = new uint8_t[size*components];
    neopixel.updateLength(size);
    neopixel.updateType(pixelType);
    neopixel.setPin(PIN);

    // Done
    sendResponse("OK");
}

void commandSetBrightness() {
    Serial.println(F("Command: SetBrightness"));

    // Read value
    uint8_t brightness = ble.read();

    // Set brightness
    neopixel.setBrightness(brightness);

    // Refresh pixels
    swapBuffers();

    // Done
    sendResponse("OK");
}

void commandClearColor() {
    Serial.println(F("Command: ClearColor"));

    // Read color
    uint8_t color[MAXCOMPONENTS];
    for (int j = 0; j < components;) {
        if (ble.available()) {
            color[j] = ble.read();
            j++;
        }
    }

    // Set all leds to color
    int size = width * height;
    uint8_t *base_addr = pixelBuffer;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < components; j++) {
            *base_addr = color[j];
            base_addr++;
        }
    }

    // Swap buffers
    Serial.println(F("ClearColor completed"));
    swapBuffers();


    if (components == 3) {
        serial_printf("\tclear (%d, %d, %d)\n", color[0], color[1], color[2] );
    }
    else {
        serial_printf("\tclear (%d, %d, %d, %d)\n", color[0], color[1], color[2], color[3] );
    }

    // Done
    sendResponse("OK");
}

void commandSetPixel() {
    Serial.println(F("Command: SetPixel"));

    // Read position
    uint8_t x = ble.read();
    uint8_t y = ble.read();

    // Read colors
    uint32_t pixelOffset = y*width+x;
    uint32_t pixelDataOffset = pixelOffset*components;
    uint8_t *base_addr = pixelBuffer+pixelDataOffset;
    for (int j = 0; j < components;) {
        if (ble.available()) {
            *base_addr = ble.read();
            base_addr++;
            j++;
        }
    }

    // Set colors
    uint32_t neopixelIndex = y*stride+x;
    uint8_t *pixelBufferPointer = pixelBuffer + pixelDataOffset;
    uint32_t color;
    if (components == 3) {
        color = neopixel.Color( *pixelBufferPointer, *(pixelBufferPointer+1), *(pixelBufferPointer+2) );
        serial_printf("\tcolor (%d, %d, %d)\n",*pixelBufferPointer, *(pixelBufferPointer+1), *(pixelBufferPointer+2) );
    }
    else {
        color = neopixel.Color( *pixelBufferPointer, *(pixelBufferPointer+1), *(pixelBufferPointer+2), *(pixelBufferPointer+3) );
        serial_printf("\tcolor (%d, %d, %d, %d)\n", *pixelBufferPointer, *(pixelBufferPointer+1), *(pixelBufferPointer+2), *(pixelBufferPointer+3) );
    }
    neopixel.setPixelColor(neopixelIndex, color);
    neopixel.show();

    // Done
    sendResponse("OK");
}

void commandImage() {
    serial_printf("Command: Image %dx%d, %d, %d\n", width, height, components, stride);

    // Receive new pixel buffer
    int size = width * height;
    uint8_t *base_addr = pixelBuffer;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < components;) {
            if (ble.available()) {
                *base_addr = ble.read();
                base_addr++;
                j++;
            }
        }

/*
    if (components == 3) {
      uint32_t index = i*components;
      Serial.printf("\tp%d (%d, %d, %d)\n", i, pixelBuffer[index], pixelBuffer[index+1], pixelBuffer[index+2] );
    }
    */
    }

    // Swap buffers
    Serial.println(F("Image received"));
    swapBuffers();

    // Done
    sendResponse("OK");
}






void setup() {
    Serial.begin(115200);
    unsigned long t = millis();
    while (!Serial.available() && ((millis() - t) < 5000)); // wait max 5 seconds
    delay(5000);

    diveController.setup();


    /* Initialise the module */
    Serial.print(F("Initialising the Bluefruit LE module: "));

    if ( !ble.begin(VERBOSE_MODE) )
    {
        error(F("Couldn't find Bluefruit, make sure it's in CoMmanD mode & check wiring?"));
    }
    Serial.println( F("OK!") );

    /* Disable command echo from Bluefruit */
    ble.echo(false);

    Serial.println("Requesting Bluefruit info:");
    /* Print Bluefruit information */
    ble.info();

    Serial.println(F("***********************"));

    // Set Bluefruit to DATA mode
    Serial.println( F("Switching to DATA mode!") );
    ble.setMode(BLUEFRUIT_MODE_DATA);

    Serial.println(F("***********************"));


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

/*void loop() {
    diveController.step();
}*/


void loop()
{
    // Echo received data
    if ( ble.isConnected() )
    {
        int command = ble.read();

        switch (command) {
            case 'V': {   // Get Version
                commandVersion();
                break;
            }

            case 'S': {   // Setup dimensions, components, stride...
                commandSetup();
                break;
            }

            case 'C': {   // Clear with color
                commandClearColor();
                break;
            }

            case 'B': {   // Set Brightness
                commandSetBrightness();
                break;
            }

            case 'P': {   // Set Pixel
                commandSetPixel();
                break;
            }

            case 'I': {   // Receive new image
                commandImage();
                break;
            }

        }
    }
}







