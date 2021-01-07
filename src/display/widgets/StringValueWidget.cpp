#include "StringValueWidget.h"


StringValueWidget::StringValueWidget(Display *display, char const *title, uint16_t leftX, uint16_t topY, uint16_t width, uint16_t height, uint8_t align, bool hidden)
        : Widget(display, leftX, topY, width, height, hidden) {
    _title = title;
    strcpy(_value,"--");
    _align = align;
    update();
}


void StringValueWidget::updateValue(char const *value) {
    bool changed = (strcmp(_value, value) != 0);
    if (changed) {
        strcpy(_value, value);
    }
    if (_hidden) {
        show();
    } else if (changed) { // only update display if something changed
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

    _display->fillWithBackground(_leftX - 1, _topY + 20, _width, (_height - 20)); // clear background for the value only as it is the only part that is redrawn
    _display->drawBigValueString(_value, _leftX, _topY + _height, _width, _align);
    //_display->drawRectangle(_leftX,_topY,_width, _height); // shows bounding box for debugging
}


void StringValueWidget::redraw() {
    _drawTitle = true; // make sure that if the widget is updated the title will be redrawn too.
    Widget::update();
}





