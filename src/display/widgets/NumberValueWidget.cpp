#include "NumberValueWidget.h"

NumberValueWidget::NumberValueWidget(Display *display, char const *title, uint8_t numberOfDecimals, uint16_t leftX, uint16_t topY, uint16_t width, uint16_t height, uint8_t align, bool hidden) :
        Widget(display, leftX, topY, width, height, hidden) {
    _title = title;
    _numberOfDecimals = numberOfDecimals;
    _value = 0.0;
    _align = align;

    update();
}

void NumberValueWidget::redraw() {
    _drawTitle = true; // make sure that if the widget is updated the title will be redrawn too.
    update();
}

void NumberValueWidget::updateValue(double value) {
    _hideValue = false;
    if (_hidden) {
        _value = value;
        show();
    } else if (_value != value) { // only update display if something changed
        _value = value;
        update();
    }
}

void NumberValueWidget::hideValue() {
    _display->fillWithBackground(_leftX, _topY + 20, _width, (_height - 20)); // clear background for the value only as it is the only part that is redrawn
}

void NumberValueWidget::show() {
    _drawTitle = true; // make sure that if the widget is shown again the title will be redrawn too.
    Widget::show();
}

void NumberValueWidget::draw() {
    if (_drawTitle) { // draw the title only the first time this widget is drawn
        _display->drawTitleString(_title, _leftX, _topY + 20, _width);
        _drawTitle = false;
    }

    hideValue(); // clear background for the value only as it is the only part that is redrawn
    _display->drawBigValueNumber(_value, _numberOfDecimals, _leftX, _topY + _height, _width, _align);
    //_display->drawRectangle(_leftX,_topY,_width, _height); // shows bounding box for debugging
}




