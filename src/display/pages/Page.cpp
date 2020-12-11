#include "Page.h"


Page::Page(uint8_t pageNr, Display *display, DecoManager *decoManager) {
    _pageNr = pageNr;
    _display = display;
    _dive = decoManager->getDive();
    _gasManager = decoManager->getGasManager();
    _decoManager = decoManager;
}

Page::~Page() = default;

uint8_t Page::getPageNr() {
    return _pageNr;
}

void Page::clear() {
    _display->clear();
}

uint8_t Page::handleLeftButtonPress() {
    Serial.print(F("OVERIDE ME! Left button pressed in page: "));
    Serial.println(_pageNr);
    return _pageNr;
}

uint8_t Page::handleRightButtonPress() {
    Serial.print(F("OVERIDE ME! Right button pressed in page: "));
    Serial.println(_pageNr);
    return _pageNr;
}





