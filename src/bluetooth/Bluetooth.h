#ifndef DIVEC_BLUETOOTH_H
#define DIVEC_BLUETOOTH_H

#include "Adafruit_BluefruitLE_SPI.h"
#include "bluetooth/BluefruitConfig.h"

class Bluetooth {
public:
    ~Bluetooth();

    bool init();

    void receive();

private:
    Adafruit_BluefruitLE_SPI *_bleSpiFriend;
};


#endif //DIVEC_BLUETOOTH_H
