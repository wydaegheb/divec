#ifndef DIVEC_EDITABLE_VALUE_WIDGET_H
#define DIVEC_EDITABLE_VALUE_WIDGET_H

#include <display/widgets/Widget.h>

class EditableDigitWidget : public Widget {
public:

    EditableDigitWidget(Display *display, char const *label, uint8_t currentValue, uint8_t minValue, uint8_t maxValue, uint16_t leftX, uint16_t topY, uint16_t width,
                        uint16_t height);

    void select();

    void save();

    void nextValue();

    uint8_t getValue() const;

    void draw() override;

    void redraw() override;

    bool isSelected() const;

    EditableDigitWidget *handleLeftButtonPress();

    EditableDigitWidget *handleRightButtonPress();

private:

    bool _selected = false;
    bool _drawLabel = true;
    bool _drawValue = true;


    char const *_label;

    uint8_t _leftXLabel;

    uint8_t _minValue;
    uint8_t _maxValue;
    uint8_t _currentValue;
    uint8_t _nrOfDigits;

};

#endif