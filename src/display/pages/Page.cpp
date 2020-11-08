#include "Page.h"


Page::Page(char const* pageName, Display *display, DecoManager *decoManager) {
    _pageName = pageName;
    _display = display;
    _dive = decoManager->getCurrentDive();
    _gasManager = decoManager->getGasManager();
    _decoManager = decoManager;
}

Page::~Page() = default;

char const* Page::getPageName() {
    return _pageName;
}

void Page::clear() {
    _display->clear();
}

char const* Page::handleLeftButtonPress() {
    Serial.print(F("OVERIDE ME! Left button pressed in page: "));
    Serial.println(_pageName);
    return _pageName;
}

char const* Page::handleRightButtonPress() {
    Serial.print(F("OVERIDE ME! Right button pressed in page: "));
    Serial.println(_pageName);
    return _pageName;
}





