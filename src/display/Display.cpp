#include "Display.h"

////////////////////////////////////////////////////////
// DISPLAY
////////////////////////////////////////////////////////

Display::Display(uint8_t bufferSize) : BaseMenuRenderer(bufferSize) {
    _config = new DisplayConfig();
    _tft = new Adafruit_ILI9341(TFT_CS, TFT_DC);
}

Display::~Display() = default;

void Display::init() {
    Serial.println(F("Initializing display."));

    _tft->begin();
    _tft->setRotation(1);
    _tft->fillScreen(BLACK);

    prepareConfig();

    clear();

    Serial.println(F(" - display initialized."));
}

void Display::clear() {
    fillWithBackground(0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT);
}

void Display::fillWithBackground(uint16_t leftX, uint16_t topY, uint16_t width, uint16_t height) {
    // based on void Adafruit_GFX::drawRGBBitmap(int16_t x, int16_t y, uint16_t *bitmap, int16_t w, int16_t h)
    // BUT this is nearly 40 times! faster than calling writePixel for every pixel. Reduces "flickering" drastically!
    // background image is stored in progmem (see display/bgimage/jellyfish.h) to avoid out of memory issues
    _tft->startWrite();
    _tft->setAddrWindow(leftX, topY, width, height);
    for (uint16_t y = topY; y < (topY + height); y++) {
        for (uint16_t x = leftX; x < (leftX + width); x++) {
            _tft->SPI_WRITE16(pgm_read_word(jellyFishBitmap + x + y * DISPLAY_WIDTH));
        }
    }
    _tft->endWrite();
}

void Display::drawTitleString(char const *title, uint16_t leftX, uint16_t bottomY, uint16_t width) {
    drawTitleString(title, leftX, bottomY, width, ALIGN_LEFT);
}

void Display::drawTitleString(char const *title, uint16_t leftX, uint16_t bottomY, uint16_t width, uint8_t align) {
    _tft->setFont(_config->titleFont);
    _tft->setTextColor(Settings::TITLE_COLOR);
    drawAlignedString(title, leftX, bottomY, width, align);
}

void Display::drawBigValueNumber(double value, uint8_t numberOfDecimals, uint16_t leftX, uint16_t bottomY, uint16_t width) {
    drawBigValueNumber(value, numberOfDecimals, leftX, bottomY, width, ALIGN_RIGHT);
}

void Display::drawBigValueNumber(double value, uint8_t numberOfDecimals, uint16_t leftX, uint16_t bottomY, uint16_t width, uint8_t align) {
    _tft->setFont(_config->bigValueFont);
    _tft->setTextColor(Settings::VALUE_COLOR);

    char valStr[numberOfDecimals + 5];
    snprintf(valStr, numberOfDecimals + 5, "%.*f", numberOfDecimals, value);

    drawAlignedString(valStr, leftX, bottomY, width, align);
}

void Display::drawBigValueString(char const *value, uint16_t leftX, uint16_t bottomY, uint16_t width) {
    drawBigValueString(value, leftX, bottomY, width, ALIGN_RIGHT);
}

void Display::drawBigValueString(char const *value, uint16_t leftX, uint16_t bottomY, uint16_t width, uint8_t align) {
    _tft->setFont(_config->bigValueFont);
    _tft->setTextColor(Settings::VALUE_COLOR);
    drawAlignedString(value, leftX, bottomY, width, align);
}

void Display::drawAlignedString(char const *s, uint16_t leftX, uint16_t bottomY, uint16_t width, uint16_t align) {
    uint16_t calculatedLeft;
    uint16_t calculatedBottom = bottomY - 2; // keep 2px margins
    if (align == ALIGN_LEFT) {
        calculatedLeft = leftX + 2; // keep 2px margins
    } else {
        int16_t x1, y1;
        uint16_t w, h;
        _tft->getTextBounds(s, leftX, bottomY, &x1, &y1, &w, &h); //calc width of new string
        if (align == ALIGN_CENTER) {
            calculatedLeft = leftX + width / 2 - w / 2;
        } else if (align == ALIGN_RIGHT) {
            calculatedLeft = leftX + width - w - 4 - 2; // keep 2px margins
        }
    }
    _tft->setCursor(calculatedLeft, calculatedBottom);
    _tft->print(s);
}

void Display::drawBattery(uint8_t percentage, uint16_t leftX, uint16_t topY, uint16_t width, uint16_t height) {
    uint16_t batH = ((height - 6) * percentage) / 100;

    _tft->drawFastHLine(leftX + (width / 2) - 4, topY, 8, Settings::TITLE_COLOR);
    _tft->drawFastHLine(leftX + (width / 2) - 4, topY + 1, 8, Settings::TITLE_COLOR);
    _tft->drawRoundRect(leftX, topY + 2, width, height - 2, 1, Settings::TITLE_COLOR);
    _tft->fillRoundRect(leftX + 2, topY + (height - 2 - batH), width - 4, batH, 1, Settings::TITLE_COLOR);
}

void Display::drawRectangle(uint16_t leftX, uint16_t topY, uint16_t width, uint16_t height) {
    _tft->drawRect(leftX, topY, width, height, Settings::TITLE_COLOR);
}


// tc menu functions

Adafruit_GFX *Display::getGraphics() { return _tft; }

DisplayConfig *Display::getGfxConfig() { return _config; }

BaseDialog *Display::getDialog() {
    if (dialog == nullptr) {
        dialog = new Dialog();
    }
    return dialog;
}

void Display::renderTitleArea() {
    if (menuMgr.getCurrentMenu() == menuMgr.getRoot()) {
        safeProgCpy(buffer, SETTINGS_TEXT, bufferSize);
    } else {
        menuMgr.getCurrentMenu()->copyNameToBuffer(buffer, bufferSize);
    }

    serdebugF3("Render title, fontMag: ", buffer, gfxConfig->titleFontMagnification);

    _tft->setFont(_config->titleFont);
    _tft->setTextSize(_config->titleFontMagnification);

    int fontYStart = _config->titlePadding.top;
    Coord extents = textExtents(buffer, 0, _config->titleFont ? _tft->height() : 0);
    titleHeight = extents.y + _config->titlePadding.top + _config->titlePadding.bottom;
    if (_config->titleFont) {
        fontYStart = titleHeight - (_config->titlePadding.bottom);
    }

    serdebugF3("titleHeight, fontYStart: ", titleHeight, fontYStart);

    _tft->fillRect(0, 0, _tft->width(), titleHeight, _config->bgTitleColor);
    _tft->setTextColor(_config->fgTitleColor);
    _tft->setCursor(_config->titlePadding.left, fontYStart);
    _tft->print(buffer);
    titleHeight += _config->titleBottomMargin;
}

bool Display::renderWidgets(bool forceDraw) {
    TitleWidget *widget = firstWidget;
    int xPos = _tft->width() - _config->widgetPadding.right;
    bool redrawNeeded = forceDraw;
    while (widget) {
        xPos -= widget->getWidth();

        if (widget->isChanged() || forceDraw) {
            redrawNeeded = true;

            serdebugF3("Drawing widget pos,icon: ", xPos, widget->getCurrentState());

            _tft->fillRect(xPos, _config->widgetPadding.top, widget->getWidth(), widget->getHeight(), _config->bgTitleColor);

            _tft->drawXBitmap(xPos, _config->widgetPadding.top, widget->getCurrentIcon(), widget->getWidth(), widget->getHeight(),
                              _config->widgetColor);
        }

        widget = widget->getNext();
        xPos -= _config->widgetPadding.left;
    }
    return redrawNeeded;
}

void Display::renderListMenu(int titleHeight) {
    ListRuntimeMenuItem *runList = reinterpret_cast<ListRuntimeMenuItem *>(menuMgr.getCurrentMenu());

    uint8_t maxY = uint8_t((_tft->height() - titleHeight) / itemHeight);
    maxY = min(maxY, runList->getNumberOfParts());
    uint8_t currentActive = runList->getActiveIndex();

    uint8_t offset = 0;
    if (currentActive >= maxY) {
        offset = (currentActive + 1) - maxY;
    }

    int yPos = titleHeight;
    for (int i = 0; i < maxY; i++) {
        uint8_t current = offset + i;
        RuntimeMenuItem *toDraw = (current == 0) ? runList->asBackMenu() : runList->getChildItem(current - 1);
        renderMenuItem(yPos, itemHeight, toDraw);
        yPos += itemHeight;
    }

    // reset the list item to a normal list again.
    runList->asParent();
}

void Display::render() {
    uint8_t locRedrawMode = redrawMode;
    redrawMode = MENUDRAW_NO_CHANGE;

    countdownToDefaulting();

    if (locRedrawMode == MENUDRAW_COMPLETE_REDRAW) {
        // pre-populate the font.
        _tft->setFont(_config->itemFont);
        _tft->setTextSize(_config->itemFontMagnification);
        int yLocation = _config->itemFont ? _tft->height() : 0;
        Coord itemExtents = textExtents("Aaygj", _config->itemPadding.left, yLocation);

        itemHeight = itemExtents.y + _config->itemPadding.top + _config->itemPadding.bottom;
        serdebugF2("Redraw all, new item height ", itemHeight);

        _tft->fillScreen(_config->bgItemColor);
        renderTitleArea();

        taskManager.yieldForMicros(0);

        renderWidgets(true);

        taskManager.yieldForMicros(0);
    } else {
        renderWidgets(false);
    }

    _tft->setFont(_config->itemFont);
    _tft->setTextSize(_config->itemFontMagnification);
    int maxItemsY = ((_tft->height() - titleHeight) / itemHeight);

    if (menuMgr.getCurrentMenu()->getMenuType() == MENUTYPE_RUNTIME_LIST) {
        if (menuMgr.getCurrentMenu()->isChanged() || locRedrawMode != MENUDRAW_NO_CHANGE) {
            renderListMenu(titleHeight);
        }
    } else {
        MenuItem *item = menuMgr.getCurrentMenu();
        int activeOffset = offsetOfCurrentActive(item);
        // first we find the first currently active item in our single linked list
        if (activeOffset >= maxItemsY) {
            uint8_t toOffsetBy = (activeOffset - maxItemsY) + 1;

            if (lastOffset != toOffsetBy) locRedrawMode = MENUDRAW_COMPLETE_REDRAW;
            lastOffset = toOffsetBy;

            while (item != nullptr && toOffsetBy) {
                if (item->isVisible()) toOffsetBy = toOffsetBy - 1;
                item = item->getNext();
            }
        } else {
            if (lastOffset != 0xff) locRedrawMode = MENUDRAW_COMPLETE_REDRAW;
            lastOffset = 0xff;
        }

        // and then we start drawing items until we run out of screen or items
        int ypos = titleHeight;
        while (item && (ypos + itemHeight) <= _tft->height()) {
            if (item->isVisible()) {
                if (locRedrawMode != MENUDRAW_NO_CHANGE || item->isChanged()) {
                    taskManager.yieldForMicros(0);
                    renderMenuItem(ypos, itemHeight, item);
                }
                ypos += itemHeight;
            }
            item = item->getNext();
        }
    }
}

void Display::renderMenuItem(int yPos, int menuHeight, MenuItem *item) {
    int icoWid = _config->editIconWidth;
    int icoHei = _config->editIconHeight;

    item->setChanged(false); // we are drawing the item so it's no longer changed.

    int imgMiddleY = yPos + ((menuHeight - icoHei) / 2);
    if (item->isEditing()) {
        _tft->fillRect(0, yPos, _tft->width(), menuHeight, _config->bgSelectColor);
        _tft->drawXBitmap(_config->itemPadding.left, imgMiddleY, _config->editIcon, icoWid, icoHei, _config->fgSelectColor);
        _tft->setTextColor(_config->fgSelectColor);
        serdebugF("Item Editing");
    } else if (item->isActive()) {
        _tft->setTextColor(_config->fgSelectColor);
        _tft->fillRect(0, yPos, _tft->width(), menuHeight, _config->bgSelectColor);
        _tft->drawXBitmap(_config->itemPadding.left, imgMiddleY, _config->activeIcon, icoWid, icoHei, _config->fgSelectColor);
        serdebugF("Item Active");
    } else {
        _tft->fillRect(0, yPos, _tft->width(), menuHeight, _config->bgItemColor);
        _tft->setTextColor(_config->fgItemColor);
        serdebugF("Item Normal");
    }

    taskManager.yieldForMicros(0);

    int textPos = _config->itemPadding.left + icoWid + _config->itemPadding.left;

    int drawingPositionY = yPos + _config->itemPadding.top;
    if (_config->itemFont) {
        drawingPositionY = yPos + (menuHeight - (_config->itemPadding.bottom));
    }
    _tft->setCursor(textPos, drawingPositionY);
    item->copyNameToBuffer(buffer, bufferSize);

    serdebugF4("Printing menu item (name, ypos, drawingPositionY)", buffer, yPos, drawingPositionY);

    _tft->print(buffer);

    if (isItemActionable(item)) {
        int rightOffset = _tft->width() - (_config->itemPadding.right + icoWid);
        _tft->drawXBitmap(rightOffset, imgMiddleY, _config->activeIcon, icoWid, icoHei, _config->fgSelectColor);
        buffer[0] = 0;
    } else if (item->getMenuType() == MENUTYPE_BACK_VALUE) {
        safeProgCpy(buffer, MENU_BACK_TEXT, bufferSize);
    } else {
        menuValueToText(item, JUSTIFY_TEXT_LEFT);
    }
    Coord coord = textExtents(buffer, textPos, yPos);
    int16_t right = _tft->width() - (coord.x + _config->itemPadding.right);
    _tft->setCursor(right, drawingPositionY);
    _tft->print(buffer);
    serdebugF2("Value ", buffer);

    taskManager.yieldForMicros(0);
}


void Display::prepareConfig() {
    makePadding(_config->titlePadding, 5, 5, 20, 5);
    makePadding(_config->itemPadding, 5, 5, 6, 5);
    makePadding(_config->widgetPadding, 5, 10, 0, 5);

    _config->bgTitleColor = RGB(255, 255, 255);
    _config->fgTitleColor = RGB(0, 0, 0);
    _config->titleFont = &NotoSans_Condensed14pt7b;
    _config->titleBottomMargin = 10;

    _config->bgItemColor = RGB(0, 0, 0);
    _config->fgItemColor = RGB(255, 255, 255);
    _config->itemFont = &NotoSans_Condensed12pt7b;

    _config->bigValueFont = &NotoSans_Condensed24pt7b;

    _config->bgSelectColor = RGB(100, 100, 100);
    _config->fgSelectColor = RGB(255, 255, 255);
    _config->widgetColor = RGB(30, 30, 30);

    //_config->titleFontMagnification = 4;
    //_config->itemFontMagnification = 2;
    _config->titleFontMagnification = 1;
    _config->itemFontMagnification = 1;

    if (_config->editIcon == nullptr || _config->activeIcon == nullptr) {
        _config->editIcon = defEditingIcon;
        _config->activeIcon = defActiveIcon;
        _config->editIconWidth = 16;
        _config->editIconHeight = 12;
    }

}

Coord Display::textExtents(const char *text, int16_t x, int16_t y) {
    int16_t x1, y1;
    uint16_t w, h;
    _tft->getTextBounds((char *) text, x, y, &x1, &y1, &w, &h);

    serdebugF4("Textbounds (y1, w, h): ", y1, w, h);
    return Coord(w, h);
}

////////////////////////////////////////////////////////
// DIALOG
////////////////////////////////////////////////////////


Dialog::Dialog() : BaseDialog() {
    bitWrite(flags, DLG_FLAG_SMALLDISPLAY, false);
}

void Dialog::internalRender(int currentValue) {
    Display *display = reinterpret_cast<Display *>(MenuRenderer::getInstance());
    DisplayConfig *config = display->getGfxConfig();
    Adafruit_GFX *_tft = display->getGraphics();

    if (needsDrawing == MENUDRAW_COMPLETE_REDRAW) {
        _tft->fillScreen(config->bgItemColor);
    }

    _tft->setFont(config->itemFont);
    _tft->setTextSize(config->itemFontMagnification);

    char data[20];
    safeProgCpy(data, headerPgm, sizeof(data));

    int fontYStart = config->itemPadding.top;
    Coord extents = display->textExtents(data, 0, config->itemFont ? _tft->height() : 0);
    int dlgNextDraw = extents.y + config->titlePadding.top + config->titlePadding.bottom;
    if (config->itemFont) {
        fontYStart = dlgNextDraw - (config->titlePadding.bottom);
    }

    _tft->fillRect(0, 0, _tft->width(), dlgNextDraw, config->bgTitleColor);
    _tft->setTextColor(config->fgTitleColor);
    _tft->setCursor(config->titlePadding.left, fontYStart);
    _tft->print(data);

    dlgNextDraw += config->titleBottomMargin;

    int startingPosition = dlgNextDraw;
    fontYStart = dlgNextDraw + config->itemPadding.top;
    dlgNextDraw = dlgNextDraw + extents.y + config->titlePadding.top + config->titlePadding.bottom;
    if (config->itemFont) {
        fontYStart = dlgNextDraw - (config->titlePadding.bottom);
    }
    _tft->fillRect(0, startingPosition, _tft->width(), dlgNextDraw, config->bgItemColor);
    _tft->setTextColor(config->fgItemColor);
    _tft->setCursor(config->titlePadding.left, fontYStart);

    _tft->print(MenuRenderer::getInstance()->getBuffer());

    bool active;
    if (button1 != BTNTYPE_NONE) {
        active = copyButtonText(data, 0, currentValue);
        drawButton(_tft, config, data, 0, active);
    }
    if (button2 != BTNTYPE_NONE) {
        active = copyButtonText(data, 1, currentValue);
        drawButton(_tft, config, data, 1, active);
    }
}

void Dialog::drawButton(Adafruit_GFX *tft, DisplayConfig *config, const char *title, uint8_t num, bool active) {
    Display *display = reinterpret_cast<Display *>(MenuRenderer::getInstance());
    Coord extents = display->textExtents(title, 0, config->itemFont ? tft->height() : 0);
    int itemHeight = (extents.y + config->itemPadding.top + config->itemPadding.bottom);
    int start = tft->height() - itemHeight;
    int fontYStart = start + config->itemPadding.top;
    if (config->itemFont) {
        fontYStart += extents.y;
    }
    int buttonWidth = tft->width() / 2;
    int xOffset = (num == 0) ? 0 : buttonWidth;
    tft->fillRect(xOffset, start, buttonWidth, itemHeight, active ? config->bgSelectColor : config->bgItemColor);
    tft->setTextColor(active ? config->fgSelectColor : config->fgItemColor);
    tft->setCursor(xOffset + ((buttonWidth - extents.x) / 2), fontYStart);
    tft->print(title);
}











