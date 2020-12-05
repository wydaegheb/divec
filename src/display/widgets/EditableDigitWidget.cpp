#include "EditableDigitWidget.h"

EditableDigitWidget::EditableDigitWidget(Display *display, char const *label, uint8_t currentValue, uint8_t minValue, uint8_t maxValue, uint16_t leftX, uint16_t topY,
                                         uint16_t width, uint16_t height) : Widget(display, leftX, topY, width, height) {
    _display = display;
    _minValue = minValue;
    _maxValue = maxValue;
    _currentValue = currentValue;
    _label = label;
    if (_maxValue > 100) {
        _nrOfDigits = 3;
    } else if (_maxValue > 10) {
        _nrOfDigits = 2;
    } else {
        _nrOfDigits = 1;
    }
    _leftXLabel = _leftX + (_width / 4) * 3;
}

void EditableDigitWidget::select() {
    _selected = true;
    _drawValue = true;
    draw();
}

void EditableDigitWidget::save() {
    _selected = false;
    draw();
}

void EditableDigitWidget::nextValue() {
    if (_currentValue < _maxValue) {
        _currentValue++;
    } else {
        _currentValue = _minValue;
    }
    _drawValue = true;
    draw();
}

uint8_t EditableDigitWidget::getValue() const {
    return _currentValue;
}

void EditableDigitWidget::redraw() {
    _drawLabel = true; // make sure that if the widget is updated the label will be redrawn too.
    _drawValue = true; // make sure that if the widget is updated the value will be redrawn too.
    update();
}

void EditableDigitWidget::draw() {
    if (_drawLabel) { // draw the title only the first time this widget is drawn
        _display->drawTitleString(_label, _leftX, _topY + _height - 5, (_width / 4) * 3);
        _drawLabel = false;
    }
    if (_drawValue) {
        _display->fillWithBackground(_leftXLabel, _topY, _width / 4, _height);
        _display->drawEditableDigit(_currentValue, _nrOfDigits, _leftXLabel, _topY + _height, _width / 4, _height, _selected);
        _drawValue = false;
    }
}

EditableDigitWidget *EditableDigitWidget::handleRightButtonPress() {
    save();
    return nullptr;
}

EditableDigitWidget *EditableDigitWidget::handleLeftButtonPress() {
    nextValue();
    return this;
}

bool EditableDigitWidget::isSelected() const {
    return _selected;
}


