#ifndef DIVEC_PAGE_H
#define DIVEC_PAGE_H

#include <display/Display.h>
#include <decomanager/DecoManager.h>

#define MAIN_PAGE 1
#define SETTINGS_PAGE 2


class Page {
public:
    Page(uint8_t pageNr, Display *display, DecoManager *decoManager);

    virtual ~Page();

    uint8_t getPageNr();

    virtual void update() = 0;

    virtual void redraw() = 0;

    virtual uint8_t handleLeftButtonPress();

    virtual uint8_t handleRightButtonPress();

protected:
    virtual void clear();

    uint8_t _pageNr;
    Display *_display;
    DecoManager *_decoManager;
    GasManager *_gasManager;
    Dive *_dive;
};


#endif //DIVEC_PAGE_H
