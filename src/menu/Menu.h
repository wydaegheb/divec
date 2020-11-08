#ifndef DIVEC_MENU_H
#define DIVEC_MENU_H


#include <decomanager/DecoManager.h>
#include <display/Display.h>
#include <display/pages/Page.h>
#include <display/pages/MainPage.h>
#include <display/pages/SettingsPage.h>

class Menu {
public:
    ~Menu();

    void init(Display *display, DecoManager *decoManager);

    void update();

    void handleLeftButtonPress();
    void handleRightButtonPress();

private:
    void setCurrentPage(char const* pageName);

    Page *_currentPage;
    std::list<Page*> _pages;

    DecoManager* _decoManager;
    Dive* _currentDive;
};


#endif //DIVEC_MENU_H
