#ifndef DIVEC_DIVE_DISPLAY_H
#define DIVEC_DIVE_DISPLAY_H

#include <Adafruit_GFX.h>  // Core graphics library
#include <Adafruit_ILI9341.h> // Hardware-specific library for ILI9341

#include <display/fonts/arial12pt7.h>
#include <display/fonts/arialbd10pt7.h>
#include <display/fonts/arialn24pt7b.h>

#include <display/fonts/NotoSans_Condensed12pt7b.h>
#include <display/fonts/NotoSans_Condensed9pt7b.h>
#include <display/fonts/NotoSans_Condensed24pt7b.h>

#include <domain/settings/Settings.h>


#define DISPLAY_HEIGHT 240
#define DISPLAY_WIDTH 320

#define TFT_CS 9
#define TFT_DC 10

#define ALIGN_LEFT 1
#define ALIGN_CENTER 2
#define ALIGN_RIGHT 3

class Display {
public:
    virtual ~Display();

    void init(FileSystem* fileSystem);

    void drawTitleString(char const* s, uint16_t leftX, uint16_t bottomY, uint16_t width);

    void drawBigValueString(char const* value, uint16_t leftX, uint16_t bottomY, uint16_t width);
    void drawBigValueString(char const* value, uint16_t leftX, uint16_t bottomY, uint16_t width, uint8_t align);

    void drawBigValueNumber(double value, uint8_t numberOfDecimals, uint16_t leftX, uint16_t bottomY, uint16_t width);
    void drawBigValueNumber(double value, uint8_t numberOfDecimals, uint16_t leftX, uint16_t bottomY, uint16_t width, uint8_t align);

    void drawEditableDigit(int currentValue, uint16_t leftX, uint16_t bottomY, uint16_t width, uint16_t height, bool selected);

    void drawBattery(uint8_t percentage, uint16_t leftX, uint16_t topY, uint16_t width, uint16_t height);

    void fillWithBackground(uint16_t leftX, uint16_t topY, uint16_t width, uint16_t height);

    void clear();


private:

    void drawAlignedString(char const* s, uint16_t leftX, uint16_t bottomY, uint16_t width, uint16_t align);

    Adafruit_ILI9341 _tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

    uint16_t *_bgImage;

    const GFXfont *_titleFont;
    const GFXfont *_smallValueFont;
    const GFXfont *_bigValueFont;

};

#endif