#ifndef DIVEC_DIVE_DISPLAY_H
#define DIVEC_DIVE_DISPLAY_H

#include <Adafruit_GFX.h>  // Core graphics library
#include <Adafruit_ILI9341.h> // Hardware-specific library for ILI9341

#include <BaseRenderers.h>
#include <BaseDialog.h>
#include <GfxMenuConfig.h>

#include <display/bgimage/jellyfish.h>
#include <domain/settings/Settings.h>

#include <display/fonts/NotoSans_Condensed12pt7b.h>
/*
#include <display/fonts/NotoSans_Condensed14pt7b.h>
*/
#include <display/fonts/NotoSans_Condensed24pt7b.h>


/*#include <display/fonts/FreeSans9pt7b.h>
#include <display/fonts/FreeSans12pt7b.h>*/
#include <display/fonts/FreeSans12pt7b.h>

#define DISPLAY_HEIGHT 240
#define DISPLAY_WIDTH 320

#define TFT_CS 9
#define TFT_DC 10

#define ALIGN_LEFT 1
#define ALIGN_CENTER 2
#define ALIGN_RIGHT 3


const char MENU_BACK_TEXT[] PROGMEM = "[..]";
const char SETTINGS_TEXT[] PROGMEM = "SETTINGS";
const char DEFINE_GASSES_TEXT[] PROGMEM = "DEFINE GASSES";
const char BUTTON_NEXT_TEXT[] PROGMEM = "Next";
const char BUTTON_EDIT_TEXT[] PROGMEM = "Edit";
const char BUTTON_OK_TEXT[] PROGMEM = "Ok";
const char BUTTON_CHANGE_TEXT[] PROGMEM = "Change";
const char BUTTON_SAVE_TEXT[] PROGMEM = "Save";

enum ButtonHint : uint8_t {
    BUTTON_HINT_NONE = 0,
    BUTTON_HINT_NEXT = 1,
    BUTTON_HINT_EDIT = 2,
    BUTTON_HINT_OK = 3,
    BUTTON_HINT_CHANGE = 4,
    BUTTON_HINT_SAVE = 5
};

// display menu config
typedef struct ColorGfxMenuConfig<const GFXfont *> DisplayConfig;

class Display : public BaseMenuRenderer {
public:
    explicit Display(uint8_t bufferSize = 45);

    ~Display() override;

    void init();

    void drawSystemError(char const *error);

    void drawTitleString(char const *s, uint16_t leftX, uint16_t bottomY, uint16_t width);

    void drawTitleString(char const *s, uint16_t leftX, uint16_t bottomY, uint16_t width, uint8_t align);

    void drawBigValueString(char const *value, uint16_t leftX, uint16_t bottomY, uint16_t width);

    void drawBigValueString(char const *value, uint16_t leftX, uint16_t bottomY, uint16_t width, uint8_t align);

    void drawBottomMenuString(char const *value, uint16_t leftX, uint16_t bottomY, uint16_t width, uint8_t align);

    void drawBigValueNumber(double value, uint8_t numberOfDecimals, uint16_t leftX, uint16_t bottomY, uint16_t width);

    void drawBigValueNumber(double value, uint8_t numberOfDecimals, uint16_t leftX, uint16_t bottomY, uint16_t width, uint8_t align);

    void drawBattery(uint8_t percentage, uint16_t leftX, uint16_t topY, uint16_t width, uint16_t height);

    void drawRectangle(uint16_t leftX, uint16_t topY, uint16_t width, uint16_t height);

    void drawButtonHints();

    void fillWithBackground(uint16_t leftX, uint16_t topY, uint16_t width, uint16_t height);

    void fillWithColor(uint16_t leftX, uint16_t topY, uint16_t width, uint16_t height, uint16_t color);

    void clear();

    // tcmenu
    void render() override;

    BaseDialog *getDialog() override;

    Adafruit_GFX *getGraphics();

    DisplayConfig *getGfxConfig();

    void renderMenuItem(int yPos, int menuHeight, MenuItem *item);

    void renderTitleArea();

    bool renderWidgets(bool forceDraw);

    void renderListMenu(int titleHeight);

    Coord textExtents(const char *text, int16_t x, int16_t y);

    void setLeft(ButtonHint left);

    void setRight(ButtonHint right);

    void setHintsChanged(bool hintsChanged);

    void setRootMenu(uint8_t rootMenu);

private:

    void drawAlignedString(char const *s, uint16_t leftX, uint16_t bottomY, uint16_t width, uint16_t align);

    void prepareConfig();


    Adafruit_ILI9341 *_tft;
    DisplayConfig *_config;

    int16_t titleHeight;
    int16_t itemHeight;

    bool _hintsChanged;
    ButtonHint _buttonHintLeft;
    ButtonHint _buttonHintRight;

    uint8_t _rootMenu;

};

class Dialog : public BaseDialog {
public:
    Dialog();

protected:
    void internalRender(int currentValue) override;

    void drawButton(Adafruit_GFX *tft, DisplayConfig *config, const char *title, uint8_t num, bool active);
};

#endif