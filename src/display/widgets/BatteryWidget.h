#ifndef DIVEC_BATTERYWIDGET_H
#define DIVEC_BATTERYWIDGET_H

#include "Widget.h"

#define BATTERY_PIN A6

class BatteryWidget : public Widget{
public:
    BatteryWidget(Display *display, uint16_t leftX, uint16_t topY, uint16_t width, uint16_t height);

    void updateValue();
protected:
    void draw() override;

private:
    void readBattery();

    double _measuredvbat;
    uint8_t _percentage;

};


#endif //DIVEC_BATTERYWIDGET_H
