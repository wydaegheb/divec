#ifndef DIVEC_EDITABLE_VALUE_WIDGET_H
#define DIVEC_EDITABLE_VALUE_WIDGET_H

#include "Widget.h"

class EditableDigitWidget : public Widget {
public:

    EditableDigitWidget(Display *display, int16_t currentValue, uint16_t leftX, uint16_t topY, uint16_t width, uint16_t height);

    void select();

    void save();

    void nextValue();

    int16_t getValue();

    void draw() override;

private:

    bool _selected = false;

    int16_t _currentValue;
};

#endif