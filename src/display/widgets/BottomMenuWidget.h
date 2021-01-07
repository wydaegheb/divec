#ifndef DIVEC_BOTTOMMENUWIDGET_H
#define DIVEC_BOTTOMMENUWIDGET_H


#include <display/widgets/Widget.h>

class BottomMenuWidget : public Widget {
public:
    BottomMenuWidget(Display *display, uint16_t topY);

    void setText(char const *text);

    void show() override;

    void draw() override;

    void redraw() override;

private:
    uint8_t _align;
    char _text[40]; // widget can not show more than 40 chars!
};


#endif //DIVEC_BOTTOMMENUWIDGET_H
