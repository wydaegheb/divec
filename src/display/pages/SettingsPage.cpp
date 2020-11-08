#include "SettingsPage.h"

SettingsPage::SettingsPage(Display *display, DecoManager *decoManager) : Page("SETTINGS_PAGE", display, decoManager) {

}

SettingsPage::~SettingsPage() {
}

void SettingsPage::update() {
    _display->drawTitleString("SETTINGS", 100, 20, DISPLAY_WIDTH);
}

