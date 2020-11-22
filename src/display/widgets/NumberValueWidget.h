#ifndef DIVEC_NUMBER_VALUE_WIDGET_H
#define DIVEC_NUMBER_VALUE_WIDGET_H

#include "Widget.h"

class NumberValueWidget : public Widget {
public:
    NumberValueWidget(Display *display, char const *title, uint8_t numberOfDecimals, uint16_t leftX, uint16_t topY, uint16_t width, uint16_t height, uint8_t align = ALIGN_RIGHT,
                      bool hidden = false);

    void updateValue(double value);

    void show() override;

    void draw() override;

    void redraw() override;

private:
    uint8_t _align;
    bool _drawTitle = true;
    char const *_title;
    uint8_t _numberOfDecimals;
    double _value;
};

#endif