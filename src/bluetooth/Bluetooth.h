#ifndef DIVEC_BLUETOOTH_H
#define DIVEC_BLUETOOTH_H

#include <Adafruit_BluefruitLE_SPI.h>
#include <bluetooth/BluefruitConfig.h>

#define BLE_INC_TIME_ONE_MIN '5'
#define BLE_DESCENT_ONE_METER '6'
#define BLE_ASCENT_ONE_METER '7'
#define BLE_PRESS_LEFT_BUTTON '8'
#define BLE_PRESS_RIGHT_BUTTON '9'

class Bluetooth {
public:

    bool init();

    int8_t receive() const;

private:
    Adafruit_BluefruitLE_SPI *_bleSpiFriend;
};


#endif //DIVEC_BLUETOOTH_H
