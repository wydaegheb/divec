#include "BatteryWidget.h"

BatteryWidget::BatteryWidget(Display *display, uint16_t leftX, uint16_t topY, uint16_t width, uint16_t height) : Widget(display, leftX, topY, width, height) {
    readBattery();
    updateValue();
}

void BatteryWidget::readBattery() {
    _measuredvbat = analogRead(BATTERY_PIN);
    _measuredvbat *= 2.0;    // we divided by 2 (double 100K resitor built in), so multiply back
    _measuredvbat *= 3.3;    // Multiply by 3.3V, our reference voltage
    _measuredvbat /= 1024.0; // convert to voltage

    // less than 3.3V is probably going to lead to a cut off - i.e. can be considered "empty"
    // thus battery can vary from 100% = 4.2V to 0% = 3.3V -> usable range = 4.2V - 3.3V = 0.9V
    _percentage = 0;
    if (_measuredvbat - 3.3 > 0) {
        _percentage = (int) ((100.0 * (_measuredvbat - 3.3)) / 0.9);
        if (_percentage > 100) { // this could happen if the battery reports more than 4.2V
            _percentage = 100;
        }
    } // else percentage remains 0
}

void BatteryWidget::draw() {
    _display->fillWithBackground(_leftX, _topY, _width, _height);
    _display->drawBattery(_percentage, _leftX, _topY, _width, _height);
}

void BatteryWidget::updateValue() {
    readBattery();
    if (_hidden) {
        show();
    } else {
        update();
    }
}
