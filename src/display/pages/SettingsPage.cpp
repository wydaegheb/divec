#include "SettingsPage.h"

SettingsPage::SettingsPage(Display *display, DecoManager *decoManager) : Page(SETTINGS_PAGE, display, decoManager) {
    _testNr = new EditableDigitWidget(_display, "Date:", 0, 0, 9, 20, 70, 200, 25);
}

SettingsPage::~SettingsPage() = default;

void SettingsPage::update() {
    _testNr->update();
}

void SettingsPage::redraw() {
    _display->drawBigValueString("SETTINGS", 10, 50, DISPLAY_WIDTH, ALIGN_LEFT);
    _testNr->redraw();
}

uint8_t SettingsPage::handleLeftButtonPress() {
    if (!_testNr->isSelected()) {
        _testNr->select();
    } else {
        _testNr->handleLeftButtonPress();
    }
    return SETTINGS_PAGE;
}

uint8_t SettingsPage::handleRightButtonPress() {
    _testNr->handleRightButtonPress();
    return MAIN_PAGE;
}




