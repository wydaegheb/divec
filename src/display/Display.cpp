#include "Display.h"

Display::~Display() {
    delete[] _bgImage;
    _bgImage = nullptr;
}

void Display::init(FileSystem* fileSystem) {
    Serial.println(F("Initializing display."));
    _tft.begin();
    _tft.setRotation(1);
    _tft.fillScreen(BLACK);

/*
    _titleFont = &arial12pt7b;
    _smallValueFont = &arialbd10pt7b;
    _bigValueFont = &arialn24pt7b;
*/

    _titleFont = &NotoSans_Condensed12pt7b;
    _smallValueFont = &NotoSans_Condensed9pt7b;
    _bigValueFont = &NotoSans_Condensed24pt7b;

    _bgImage = fileSystem->loadBgImage();

    clear();
    Serial.println(F(" - display initialized."));

}

void Display::clear() {
    fillWithBackground(0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT);

}

void Display::drawTitleString(char const* title, uint16_t leftX, uint16_t bottomY, uint16_t width) {
    _tft.setFont(_titleFont);
    _tft.setTextColor(Settings::TITLE_COLOR);
    drawAlignedString(title, leftX, bottomY, width, ALIGN_LEFT);
}

void Display::drawBigValueNumber(double value, uint8_t numberOfDecimals, uint16_t leftX, uint16_t bottomY, uint16_t width) {
    drawBigValueNumber(value, numberOfDecimals, leftX, bottomY, width, ALIGN_RIGHT);
}

void Display::drawBigValueNumber(double value, uint8_t numberOfDecimals, uint16_t leftX, uint16_t bottomY, uint16_t width, uint8_t align) {
    _tft.setFont(_bigValueFont);
    _tft.setTextColor(Settings::VALUE_COLOR);

    char valStr[numberOfDecimals + 5];
    snprintf(valStr, numberOfDecimals + 5, "%.*f", numberOfDecimals, value);

    drawAlignedString(valStr, leftX, bottomY, width, align);
}

void Display::drawBigValueString(char const* value, uint16_t leftX, uint16_t bottomY, uint16_t width) {
    drawBigValueString(value, leftX, bottomY, width, ALIGN_RIGHT);
}

void Display::drawBigValueString(char const* value, uint16_t leftX, uint16_t bottomY, uint16_t width, uint8_t align) {
    _tft.setFont(_bigValueFont);
    _tft.setTextColor(Settings::VALUE_COLOR);
    drawAlignedString(value, leftX, bottomY, width, align);
}

void Display::drawEditableDigit(int currentValue, uint16_t leftX, uint16_t bottomY, uint16_t width, uint16_t height, bool selected) {
    String valueStr = String(currentValue);

    _tft.setFont(_smallValueFont);
    _tft.setTextColor(Settings::VALUE_COLOR);
    uint16_t adjustedBottomY = bottomY - 5; // -5 to leave room for the underline for selected values

    _tft.setCursor(leftX, adjustedBottomY);
    _tft.print(valueStr);

    // underline for selection if needed
    if (selected) {
        int16_t x1, y1;
        uint16_t w, h;
        _tft.getTextBounds(valueStr, leftX, adjustedBottomY, &x1, &y1, &w, &h); //calc boundingbox of new string
        _tft.drawFastHLine(leftX + 1, y1 + h + 1, width - 2, Settings::VALUE_COLOR);
        _tft.drawFastHLine(leftX + 1, y1 + h + 2, width - 2, Settings::VALUE_COLOR);
    }
}


void Display::drawAlignedString(char const* s, uint16_t leftX, uint16_t bottomY, uint16_t width, uint16_t align) {
    if (align == ALIGN_LEFT) {
        _tft.setCursor(leftX, bottomY);
    } else {
        int16_t x1, y1;
        uint16_t w, h;
        _tft.getTextBounds(s, leftX, bottomY, &x1, &y1, &w, &h); //calc width of new string
        if (align == ALIGN_CENTER) {
            _tft.setCursor(leftX + width / 2 - w / 2, bottomY);
        } else if (align == ALIGN_RIGHT) {
            _tft.setCursor(leftX + width - w - 4, bottomY);
        }
    }

    _tft.print(s);
}

void Display::fillWithBackground(uint16_t leftX, uint16_t topY, uint16_t width, uint16_t height) {
    // based on void Adafruit_GFX::drawRGBBitmap(int16_t x, int16_t y, uint16_t *bitmap, int16_t w, int16_t h)
    // BUT this is nearly 40 times! faster than calling writePixel for every pixel. Reduces "flickering" drastically!
    // _tft.drawRGBBitmap(leftX,topY,_bgImage,width,height);
    _tft.startWrite();
    _tft.setAddrWindow(leftX, topY, width, height);
    for (uint16_t y = topY; y < (topY + height); y++) {
        for (uint16_t x = leftX; x < (leftX + width); x++) {
            _tft.SPI_WRITE16(_bgImage[y * DISPLAY_WIDTH + x]);
        }
    }
    _tft.endWrite();

}

void Display::drawBattery(uint8_t percentage, uint16_t leftX, uint16_t topY, uint16_t width, uint16_t height) {
    uint16_t batH = ((height - 6) * percentage) / 100;

    _tft.drawFastHLine(leftX + (width / 2) - 4, topY, 8, Settings::TITLE_COLOR);
    _tft.drawFastHLine(leftX + (width / 2) - 4, topY + 1, 8, Settings::TITLE_COLOR);
    _tft.drawRoundRect(leftX, topY + 2, width, height - 2, 1, Settings::TITLE_COLOR);
    _tft.fillRoundRect(leftX + 2, topY + (height - 2 - batH), width - 4, batH, 1, Settings::TITLE_COLOR);
}









