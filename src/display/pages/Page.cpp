#include "Page.h"


Page::Page(uint8_t pageNr, Display *display, DecoManager *decoManager) {
    _pageNr = pageNr;
    _display = display;
    _dive = decoManager->getDive();
    _decoManager = decoManager;
}

Page::~Page() = default;

uint8_t Page::getPageNr() {
    return _pageNr;
}

void Page::clear() {
    _display->clear();
}





