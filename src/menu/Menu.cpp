#include "Menu.h"

Menu::~Menu() {
    for (Page *page:_pages) {
        delete page;
    }
    _pages.clear();
}

void Menu::init(Display *display, DecoManager *decoManager) {
    Serial.println(F("Initializing main menu."));
    _decoManager = decoManager;
    _currentPage = new MainPage(display, decoManager);
    _pages.emplace_back(_currentPage);
    _pages.emplace_back(new SettingsPage(display, decoManager));
    update();
    Serial.println(F(" - main menu initialized."));
}

void Menu::handleLeftButtonPress() {
    setCurrentPage(_currentPage->handleLeftButtonPress());
}

void Menu::handleRightButtonPress() {
    setCurrentPage(_currentPage->handleRightButtonPress());
}

void Menu::update() {
    _currentPage->update();
}

void Menu::setCurrentPage(char const* pageName) {
    for (Page *page:_pages) {
        if (strcmp(page->getPageName(),pageName) == 0) {
            _currentPage = page;
            break;
        }
    }
}


