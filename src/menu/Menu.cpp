#include "Menu.h"

const AnyMenuInfo PROGMEM minfoExit = {"[Exit]", 5, 0xFFFF, 0, &Menu::onExit};

const char enumStrDensity_0[] PROGMEM = "Salt";
const char enumStrDensity_1[] PROGMEM = "Fresh";
const char *const enumStrDensity[] PROGMEM = {enumStrDensity_0, enumStrDensity_1};
const EnumMenuInfo PROGMEM minfoDensity = {"Salinity", 3, 0xFFFF, 1, &Menu::onWaterDensity, enumStrDensity};

const char enumStrAlgorithm_0[] PROGMEM = "GF ZHL16B";
const char enumStrAlgorithm_1[] PROGMEM = "GF ZHL16C";
const char *const enumStrAlgorithm[] PROGMEM = {enumStrAlgorithm_0, enumStrAlgorithm_1};
const EnumMenuInfo PROGMEM minfoAlgorithm = {"Algorithm", 4, 0xFFFF, 1, &Menu::onAlgorithm, enumStrAlgorithm};

const char enumStrLastStop_0[] PROGMEM = "3 m";
const char enumStrLastStop_1[] PROGMEM = "6 m";
const char *const enumStrLastStop[] PROGMEM = {enumStrLastStop_0, enumStrLastStop_1};
const EnumMenuInfo PROGMEM minfoLastStop = {"Last Stop", 11, 0xFFFF, 1, &Menu::onLastStop, enumStrLastStop};

const char enumStrDecoStepSize_0[] PROGMEM = "3 m";
const char enumStrDecoStepSize_1[] PROGMEM = "6 m";
const char *const enumStrDecoStepSize[] PROGMEM = {enumStrDecoStepSize_0, enumStrDecoStepSize_1};
const EnumMenuInfo PROGMEM minfoDecoStepSize = {"Deco Step Size", 10, 0xFFFF, 1, &Menu::onDecoStepSize,
                                                enumStrDecoStepSize};

// define statics
Display *Menu::_display;
DecoManager *Menu::_decoManager;
EnumMenuItem *Menu::_salinityMenuItem;
EnumMenuItem *Menu::_lastStopMenuItem;
EnumMenuItem *Menu::_decoStepSizeMenuItem;
EnumMenuItem *Menu::_algorithmMenuItem;
ActionMenuItem *Menu::_returnToMainMenuItem;


void Menu::init(uint8_t okButtonPin, uint8_t nextButtonPin, Display *display, DecoManager *decoManager) {
    Serial.println(F("Initializing main menu."));
    _decoManager = decoManager;
    _currentPage = new MainPage(display, decoManager);
    _display = display;
    _lastUpdateTimeInSeconds = Time::getTime();
    update();


    // initialise back item
    _returnToMainMenuItem = new ActionMenuItem(&minfoExit, nullptr);

    // initialise "define gasses" menu
    uint8_t lastGasIdx = _decoManager->getGasManager()->getNrOfGasses() - 1;
    _gasFormattedMenuItems[lastGasIdx] = new GasFormattedMenuItem(_decoManager->getGasManager()->getGas(lastGasIdx),
                                                                  lastGasIdx, _returnToMainMenuItem);
    for (int i = lastGasIdx - 1; i >= 0; i--) {
        _gasFormattedMenuItems[i] = new GasFormattedMenuItem(_decoManager->getGasManager()->getGas(i), i,
                                                             _gasFormattedMenuItems[i + 1]);
    }

    // initialise "settings" menu
    _decoStepSizeMenuItem = new EnumMenuItem(&minfoDecoStepSize, 0, _returnToMainMenuItem);
    _lastStopMenuItem = new EnumMenuItem(&minfoLastStop, 0, _decoStepSizeMenuItem);
    _salinityMenuItem = new EnumMenuItem(&minfoDensity, 0, _lastStopMenuItem);
    _gradientFactorsFormattedMenuItem = new GradientFactorsFormattedMenuItem(101, _salinityMenuItem);
    _algorithmMenuItem = new EnumMenuItem(&minfoAlgorithm, 0, _gradientFactorsFormattedMenuItem);
    _dateTimeFormattedMenuItem = new DateTimeFormattedMenuItem(100, _algorithmMenuItem);

    // register custom drawing handler (before starting tcMenu!)
    _display->setCustomDrawingHandler(this);

    // initialise custom rotary encoder (button handler) with "wrap around"
    EncoderNextOkButton::setupNextOkButtonEncoder(nextButtonPin, okButtonPin, this);

    // start menu
    menuMgr.initWithoutInput(_display, _dateTimeFormattedMenuItem);

    Serial.println(F(" - main menu initialized."));
}

void Menu::update() {
    _currentPage->update();
}

void Menu::reset() {
    // if we get here the display has been reset because of a timeout of the user interface -> takeover display from tcMenu
    Serial.println(F(" Tcmenu timeout - we take the screen back"));
    _display->takeOverDisplay();
}

void Menu::started(BaseMenuRenderer *currentRenderer) {
    // take over display has just been called -> clear screen and redraw our current custom page
    Serial.println(F("Display transferred to us again. clear bottom menu, clear the display and redraw our custom page"));
    _displayOwner = true;
    _rootMenuItem = 0;
    _display->clear();
    _currentPage->clearBottomMenuItem();
    _currentPage->redraw();
}

void Menu::renderLoop(unsigned int currentValue, RenderPressMode okButtonClicked) { // only called when tc menu doesn't own the display (i.e. if we can render custom screens)
    if (okButtonClicked && _rootMenuItem > 0) { // we own the display but a menu is selected and the ok button is pressed -> switch to the selected menu and give control to tc menu
        Serial.println(F("renderloop - menu selected"));

        _displayOwner = false;
        if (_rootMenuItem == 1) {
            Serial.println(F("Settings menu selected"));
            menuMgr.setRootMenu(_dateTimeFormattedMenuItem);
        } else if (_rootMenuItem == 2) {
            Serial.println(F("Define gasses menu selected"));
            menuMgr.setRootMenu(_gasFormattedMenuItems[0]);
        }
        Menu::getDisplay()->setHintsChanged(true);
        Menu::getDisplay()->giveBackDisplay();  // Enter menu system (give display back to tc menu)

    } else { // update our own custom screen (only once every second)
        if ((Time::getTime() - _lastUpdateTimeInSeconds) >= 1000) {
            update();
            _lastUpdateTimeInSeconds = Time::getTime();
        }
    }
}

void Menu::nextButtonClicked(bool held) {
    if (_displayOwner) {
        _rootMenuItem++;
        _display->setRootMenu(_rootMenuItem);
        if (_rootMenuItem == 1) {
            _currentPage->setBottomMenuItem("Settings");
        } else if (_rootMenuItem == 2) {
            _currentPage->setBottomMenuItem("Define Gasses");
        } else {
            _currentPage->clearBottomMenuItem();
            _rootMenuItem = 0;
        }
    } 
}

void Menu::okButtonClicked(bool held) {
    Serial.println(F("Ok button clicked -ignore for the moment"));
}

DecoManager *Menu::getDecoManager() {
    return _decoManager;
}


Display *Menu::getDisplay() {
    return _display;
}

void CALLBACK_FUNCTION Menu::onExit(int id) {
    Serial.println(F("onExit"));
    Menu::getDisplay()->takeOverDisplay();
}

void CALLBACK_FUNCTION Menu::onWaterDensity(int id) {
    Serial.println(F("onWaterDensity"));
    Settings::SALINITY = _salinityMenuItem->getCurrentValue();
}

void CALLBACK_FUNCTION Menu::onAlgorithm(int id) {
    Serial.println(F("onAlgorithm"));
    Settings::ALGORITHM = _algorithmMenuItem->getCurrentValue();
    Menu::getDecoManager()->setCurrentAlgorithmIndex(_algorithmMenuItem->getCurrentValue());
}

void CALLBACK_FUNCTION Menu::onLastStop(int id) {
    Serial.println(F("onLastStop"));
    Settings::LAST_STOP = _lastStopMenuItem->getCurrentValue();
}

void CALLBACK_FUNCTION Menu::onDecoStepSize(int id) {
    Serial.println(F("onDecoStepSize"));
    Settings::DECO_STEP_SIZE = _decoStepSizeMenuItem->getCurrentValue();
}














