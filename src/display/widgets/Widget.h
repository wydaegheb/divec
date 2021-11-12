#ifndef DIVEC_WIDGET_H
#define DIVEC_WIDGET_H


#include <display/Display.h>

class Widget {
public:
    Widget(Display *display, uint16_t leftX, uint16_t topY, uint16_t width, uint16_t height, bool hidden = false);

    virtual ~Widget();

    void update();

    virtual void hide();

    virtual void show();

    virtual void redraw();

    virtual void draw() = 0;


protected:
    Display *_display;

    uint16_t _leftX;
    uint16_t _topY;
    uint16_t _width;
    uint16_t _height;

    bool _hidden;
};


#endif //DIVEC_WIDGET_H
