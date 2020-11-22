#include "SettingsPage.h"

SettingsPage::SettingsPage(Display *display, DecoManager *decoManager) : Page("SETTINGS_PAGE", display, decoManager) {
    _testNr = new EditableDigitWidget(_display, 1, 120, 120, 20, 20);
}

SettingsPage::~SettingsPage() = default;

void SettingsPage::update() {
    _testNr->update();
}

void SettingsPage::redraw() {
    _display->drawTitleString("SETTINGS", 100, 60, DISPLAY_WIDTH);
    _testNr->redraw();
}

char const *SettingsPage::handleLeftButtonPress() {
    return "MAIN_PAGE";
}




