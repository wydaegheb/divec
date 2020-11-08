#ifndef DIVEC_PAGE_H
#define DIVEC_PAGE_H

#include <display/Display.h>
#include <decomanager/DecoManager.h>

class Page {
public:
    Page(char const* pageName, Display *display, DecoManager *decoManager);

    virtual ~Page();

    char const* getPageName();

    virtual void update() = 0;

    virtual char const* handleLeftButtonPress();

    virtual char const* handleRightButtonPress();

protected:
    virtual void clear();

    char const* _pageName;
    Display *_display;
    DecoManager *_decoManager;
    GasManager *_gasManager;
    Dive *_dive;
};


#endif //DIVEC_PAGE_H
