#include "StringValueWidget.h"


StringValueWidget::StringValueWidget(Display *display, char const *title, uint16_t leftX, uint16_t topY, uint16_t width, uint16_t height, uint8_t align, bool hidden)
        : Widget(display, leftX, topY, width, height, hidden) {
    _title = title;
    strcpy(_value,"--");
    _align = align;
    update();
}


void StringValueWidget::updateValue(char const *value) {
    if (_hidden) {
        strcpy(_value, value);
        show();
    } else if (strcmp(_value, value) != 0) { // only update display if something changed
        strcpy(_value, value);
        update();
    }
}

void StringValueWidget::show() {
    _drawTitle = true; // make sure that if the widget is shown again the title will be redrawn too.
    Widget::show();
}

void StringValueWidget::draw() {
    if (_drawTitle) { // draw the title only the first time this widget is drawn
        _display->drawTitleString(_title, _leftX, _topY + 20, _width);
        _drawTitle = false;
    }

    _display->fillWithBackground(_leftX - 1, _topY + 22, _width + 2, (_height - 20)); // clear background for the value only as it is the only part that is redrawn
    _display->drawBigValueString(_value, _leftX, _topY + _height, _width, _align);
}


void StringValueWidget::redraw() {
    _drawTitle = true; // make sure that if the widget is updated the title will be redrawn too.
    Widget::update();
}





