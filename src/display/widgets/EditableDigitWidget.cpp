#include "EditableDigitWidget.h"

EditableDigitWidget::EditableDigitWidget(Display *display, int16_t currentValue, uint16_t leftX, uint16_t topY, uint16_t width, uint16_t height) : Widget(display, leftX, topY, width, height) {
    _display = display;
    _currentValue = currentValue;
}

void EditableDigitWidget::select() {
    _selected = true;
    draw();
}

void EditableDigitWidget::save() {
    _selected = false;
    draw();
}

void EditableDigitWidget::nextValue() {
    if (_currentValue < 9) {
        _currentValue++;
    } else {
        _currentValue = 0;
    }
    draw();
}

int16_t EditableDigitWidget::getValue() {
    return _currentValue;
}

void EditableDigitWidget::draw() {
    _display->fillWithBackground(_leftX, _topY, _width, _height);
    _display->drawEditableDigit(_currentValue, _leftX, _topY + _height, _width, _height, _selected);
}


