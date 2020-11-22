#include "MainPage.h"

MainPage::MainPage(Display *display, DecoManager *decoManager) : Page("MAIN_PAGE", display, decoManager) {
    Serial.println(F(" - Initializing main page."));
    _currentDepth = new NumberValueWidget(_display, "DEPTH", 0, 5, TOP_ROW_Y, 80, 65);
    Serial.println(F("   - _currentDepth"));
    _diveTime = new NumberValueWidget(_display, "TIME", 0, 95, TOP_ROW_Y, 60, 65);
    Serial.println(F("   - _diveTime"));
    _battery = new BatteryWidget(_display, 160, TOP_ROW_Y + 20, 15, 50);
    Serial.println(F("   - _battery"));
    _surfaceInterval = new NumberValueWidget(_display, "SURFACE", 0, 180, TOP_ROW_Y, 140, 65);
    Serial.println(F("   - _surfaceInterval"));
    _decoStopDepth = new NumberValueWidget(_display, "STOP", 0, 180, TOP_ROW_Y, 65, 65, ALIGN_RIGHT, true);
    Serial.println(F("   - _decoStopDepth"));
    _decoStopTime = new NumberValueWidget(_display, "TIME", 0, 250, TOP_ROW_Y, 60, 65, ALIGN_RIGHT, true);
    Serial.println(F("   - _decoStopTime"));

    _gasPO2 = new NumberValueWidget(_display, "GasPO2", 2, 120, MIDDLE_ROW_Y, 90, 65, ALIGN_CENTER);

    _mode = new StringValueWidget(_display, "", 15, BOTTOM_ROW_Y, 40, 65);
    _gasMix = new StringValueWidget(_display, "O2/HE", 95, BOTTOM_ROW_Y, 70, 65);
    _ndl = new NumberValueWidget(_display, "NDL", 0, 190, BOTTOM_ROW_Y, 50, 65);
    _tts = new NumberValueWidget(_display, "TTS", 0, 250, BOTTOM_ROW_Y, 60, 65);
}

MainPage::~MainPage() {
    delete _currentDepth;
    delete _diveTime;
    delete _battery;
    delete _surfaceInterval;
    delete _decoStopDepth;
    delete _decoStopTime;
    delete _gasPO2;
    delete _mode;
    delete _gasMix;
    delete _ndl;
    delete _tts;

    _currentDepth = nullptr;
    _diveTime = nullptr;
    _battery = nullptr;
    _surfaceInterval = nullptr;
    _decoStopDepth = nullptr;
    _decoStopTime = nullptr;
    _gasPO2 = nullptr;
    _mode = nullptr;
    _gasMix = nullptr;
    _ndl = nullptr;
    _tts = nullptr;
}

void MainPage::redraw() {
    _currentDepth->redraw();
    _diveTime->redraw();
    _battery->redraw();
    _surfaceInterval->redraw();
    _decoStopDepth->redraw();
    _decoStopTime->redraw();
    _gasPO2->redraw();
    _mode->redraw();
    _gasMix->redraw();
    _ndl->redraw();
    _tts->redraw();
}

void MainPage::update() {
    Gas *currentGas = _gasManager->getCurrentOcGas();
    DecompressionPlan *plan = _decoManager->getCurrentAlgorithm()->getDecoPlan(_gasManager);

    // top row
    _currentDepth->updateValue(_dive->getCurrentDepthInMeters());
    _diveTime->updateValue(_dive->getDiveTimeInSeconds() / 60.0);
    _battery->updateValue();
    if (_dive->isInProgress()) { // when diving -> show deco stops
        DecompressionStep *decoStop = plan->getStops().front();
        _surfaceInterval->hide();
        _decoStopTime->updateValue(decoStop->getDurationInSeconds() / 60.0);
        _decoStopDepth->updateValue(decoStop->getEndDepthInMeters());
    } else {
        _decoStopDepth->hide();
        _decoStopTime->hide();
        _surfaceInterval->updateValue(_decoManager->getSurfaceIntervalInSeconds() / 60.0);
    }

    // middle row
    _gasPO2->updateValue(DiveEquations::gasPressureInBars(_dive->getCurrentDepthInMeters(), currentGas->getO2()));

    // bottom row
    _mode->updateValue("OC");
    _gasMix->updateValue(currentGas->getName());
    _ndl->updateValue(_decoManager->getNdlInSeconds() / 60.0);
    _tts->updateValue(plan->getTtsInSeconds() / 60.0);
}

char const *MainPage::handleRightButtonPress() {
    return "SETTINGS_PAGE";
}


