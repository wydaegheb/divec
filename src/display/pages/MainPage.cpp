#include "MainPage.h"

MainPage::MainPage(Display *display, DecoManager *decoManager) : Page(MAIN_PAGE, display, decoManager) {
    _currentDepth = new NumberValueWidget(_display, "DEPTH", 0, 5, TOP_ROW_Y, 80, 65);
    _diveTime = new NumberValueWidget(_display, "TIME", 0, 95, TOP_ROW_Y, 60, 65);
    _battery = new BatteryWidget(_display, 160, TOP_ROW_Y + 20, 15, 50);
    _surfaceInterval = new NumberValueWidget(_display, "SURFACE", 0, 180, TOP_ROW_Y, 140, 65);
    _decoStopDepth = new NumberValueWidget(_display, "STOP", 0, 180, TOP_ROW_Y, 65, 65, ALIGN_RIGHT, true);
    _decoStopTime = new NumberValueWidget(_display, "TIME", 0, 250, TOP_ROW_Y, 60, 65, ALIGN_RIGHT, true);

    _gasPO2 = new NumberValueWidget(_display, "GasPO2", 2, 120, MIDDLE_ROW_Y, 90, 65, ALIGN_CENTER);

    _mode = new StringValueWidget(_display, "", 5, BOTTOM_ROW_Y, 65, 65);
    _gasMix = new StringValueWidget(_display, "O2/HE", 90, BOTTOM_ROW_Y, 75, 65);
    _ndl = new NumberValueWidget(_display, "NDL", 0, 190, BOTTOM_ROW_Y, 50, 65);
    _tts = new NumberValueWidget(_display, "TTS", 0, 250, BOTTOM_ROW_Y, 70, 65);
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
    DecompressionPlan *plan = _decoManager->getDecoPlan();

    // top row
    _currentDepth->updateValue(_dive->getCurrentDepthInMeters());
    _diveTime->updateValue(_dive->getDiveTimeInSeconds() / 60.0);
    _battery->updateValue();
    if (_dive->isInProgress()) { // when diving -> show deco stops
        _surfaceInterval->hide();
        DecompressionStep *decoStop = plan->getFirstStop();
        if (decoStop != nullptr) {
            _decoStopDepth->updateValue(decoStop->getEndDepthInMeters());
            _decoStopTime->updateValue(decoStop->getDurationInSeconds() / 60.0);
        } else {
            _decoStopDepth->hideValue();
            _decoStopTime->hideValue();
        }

    } else { // else show surface interval
        _decoStopDepth->hide();
        _decoStopTime->hide();
        _surfaceInterval->updateValue(_decoManager->getSurfaceIntervalInSeconds() / 60.0);
    }

    // middle row
    _gasPO2->updateValue(DiveEquations::gasPressureInBars(DiveEquations::depthInMetersToBars(_dive->getCurrentDepthInMeters()), _decoManager->getCurrentGas()->getO2()));

    // bottom row
    _mode->updateValue("OC");
    _gasMix->updateValue(_decoManager->getCurrentGas()->getName());
    _ndl->updateValue(_decoManager->getNdlInSeconds() / 60.0);
    _tts->updateValue(plan->getTtsInSeconds() / 60.0);
}



