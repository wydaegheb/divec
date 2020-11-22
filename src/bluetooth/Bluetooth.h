#ifndef DIVEC_BLUETOOTH_H
#define DIVEC_BLUETOOTH_H

#include "Adafruit_BluefruitLE_SPI.h"
#include "BluefruitConfig.h"

class Bluetooth {
public:

    bool init();

    int receive() const;

private:
    Adafruit_BluefruitLE_SPI *_bleSpiFriend;
};


#endif //DIVEC_BLUETOOTH_H
