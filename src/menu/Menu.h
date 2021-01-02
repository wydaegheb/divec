#ifndef DIVEC_MENU_H
#define DIVEC_MENU_H

#include <tcMenu.h>
#include <RuntimeMenuItem.h>
#include <SwitchInput.h>

#include <decomanager/DecoManager.h>
#include <display/Display.h>
#include <display/pages/Page.h>
#include <display/pages/MainPage.h>
#include "EncoderNextOkButton.h"

#define NEXT_BUTTON_PIN A2
#define OK_BUTTON_PIN A3



// tcMenus callback functions must always include CALLBACK_FUNCTION after the return type
#define CALLBACK_FUNCTION

class Menu : public CustomDrawing {
public:
    ~Menu() override = default;

    void init(Display *display, DecoManager *decoManager);

    void update();

    void reset() override;

    void started(BaseMenuRenderer *currentRenderer) override;

    void renderLoop(unsigned int currentValue, RenderPressMode userClick) override;

    static DecoManager *getDecoManager();

    static Display *getDisplay();

    static void CALLBACK_FUNCTION onLastStop(int id);

    static void CALLBACK_FUNCTION onDecoStepSize(int id);

    static void CALLBACK_FUNCTION onGfLow(int id);

    static void CALLBACK_FUNCTION onGfHigh(int id);

    static void CALLBACK_FUNCTION onExit(int id);

    static void CALLBACK_FUNCTION onAlgorithm(int id);

    static void CALLBACK_FUNCTION onWaterDensity(int id);

    static void CALLBACK_FUNCTION onTime(int id);

    static void CALLBACK_FUNCTION onDate(int id);

    static int CALLBACK_FUNCTION fnGassesRtCall(RuntimeMenuItem *item, uint8_t row, RenderFnMode mode, char *buffer, int bufferSize);


private:
    static DecoManager *_decoManager;
    static Display *_display;

    Page *_currentPage;
    uint32_t _lastUpdateTimeInSeconds;

    static uint8_t getMaxMenuItemIndex(MenuItem *root);

    static void onOkButtonPress(pinid_t key, bool heldDown);

    static void onNextButtonPress(pinid_t key, bool heldDown);

    static void onValueChanged(int value);
};


#endif //DIVEC_MENU_H
