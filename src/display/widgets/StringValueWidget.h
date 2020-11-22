#ifndef DIVEC_STRING_VALUE_WIDGET_H
#define DIVEC_STRING_VALUE_WIDGET_H

#include "Widget.h"

class StringValueWidget : public Widget {
public:

    StringValueWidget(Display *display, char const *title, uint16_t leftX, uint16_t topY, uint16_t width, uint16_t height, uint8_t align = ALIGN_CENTER,
                      bool hidden = false);

    void updateValue(char const *value);

    void show() override;

    void draw() override;

    void redraw() override;

private:
    uint8_t _align;
    bool _drawTitle = true;
    char const *_title;
    char _value[20]; // widget can not show more than 20 chars!
};

#endif