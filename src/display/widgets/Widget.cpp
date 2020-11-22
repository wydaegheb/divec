#include "Widget.h"

Widget::Widget(Display *display, uint16_t leftX, uint16_t topY, uint16_t width, uint16_t height,bool hidden) {
    _display = display;
    _leftX = leftX;
    _topY = topY;
    _width = width;
    _height = height;
    _hidden = hidden;
}

Widget::~Widget() = default;

void Widget::hide() {
    if (!_hidden) {
        _display->fillWithBackground(_leftX, _topY, _width, _height);
        _hidden = true;
    }
}

void Widget::show() {
    _hidden = false;
    draw();
}

void Widget::update() {
    if (!_hidden) {
        draw();
    }
}

void Widget::redraw() {
    // default - do nothing - not every widget needs a redraw
}





