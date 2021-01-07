#include "BottomMenuWidget.h"


BottomMenuWidget::BottomMenuWidget(Display *display, uint16_t topY)
        : Widget(display, 0, topY, DISPLAY_WIDTH, DISPLAY_HEIGHT - topY, true) {
    _align = ALIGN_LEFT;
    update();
}

void BottomMenuWidget::setText(char const *text) {
    bool changed = (strcmp(_text, text) != 0);
    if (changed) {
        strcpy(_text, text);
    }
    if (_hidden) {
        show();
    } else if (changed) { // only update display if something changed
        update();
    }
}

void BottomMenuWidget::show() {
    Widget::show();
}

void BottomMenuWidget::draw() {
    _display->fillWithBackground(_leftX, _topY, _width, _height); // clear background
    //_display->fillWithColor(_leftX, _topY + 20, _width, _height - 20, WHITE); // clear background
    _display->drawBottomMenuString(_text, _leftX + 5, _topY + _height - 12, _width, _align); // need to stay a bit above the bottom as some characters "stick out through the bottom" (e.g. the character g)
}


void BottomMenuWidget::redraw() {
    Widget::update();
}


