#ifndef DIVEC_MENU_H
#define DIVEC_MENU_H

#include <tcMenu.h>

#include <decomanager/DecoManager.h>
#include <display/Display.h>
#include <display/pages/Page.h>
#include <display/pages/MainPage.h>
#include <menu/encoder/EncoderNextOkButton.h>
#include <menu/menuitems/GasFormattedMenuItem.h>
#include <menu/menuitems/DateTimeFormattedMenuItem.h>
#include <menu/menuitems/GradientFactorsFormattedMenuItem.h>


#define NEXT_BUTTON_PIN A2
#define OK_BUTTON_PIN A3

// tcMenus callback functions must always include CALLBACK_FUNCTION after the return type
#define CALLBACK_FUNCTION

class Menu : public CustomDrawing, EncoderNextButtonListener {
public:
    ~Menu() override = default;

    void init(Display *display, DecoManager *decoManager);

    void update();

    void reset() override;

    void started(BaseMenuRenderer *currentRenderer) override;

    void renderLoop(unsigned int currentValue, RenderPressMode okButtonClicked) override;

    void nextButtonClicked(bool held) override;

    static DecoManager *getDecoManager();

    static Display *getDisplay();

    static void CALLBACK_FUNCTION onLastStop(int id);

    static void CALLBACK_FUNCTION onDecoStepSize(int id);

    static void CALLBACK_FUNCTION onExit(int id);

    static void CALLBACK_FUNCTION onAlgorithm(int id);

    static void CALLBACK_FUNCTION onWaterDensity(int id);

private:
    static DecoManager *_decoManager;
    static Display *_display;

    DateTimeFormattedMenuItem *_dateTimeFormattedMenuItem;
    GradientFactorsFormattedMenuItem *_gradientFactorsFormattedMenuItem;
    static EnumMenuItem *_algorithmMenuItem;
    static EnumMenuItem *_salinityMenuItem;
    static EnumMenuItem *_lastStopMenuItem;
    static EnumMenuItem *_decoStepSizeMenuItem;

    GasFormattedMenuItem *_gasFormattedMenuItems[MAX_GASSES];

    static ActionMenuItem *_returnToMainMenuItem;

    MainPage *_currentPage;
    uint32_t _lastUpdateTimeInSeconds;

    uint8_t _rootMenuItem = 0;
    bool _displayOwner = true;

};


#endif //DIVEC_MENU_H
