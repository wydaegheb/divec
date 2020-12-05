#ifndef DIVEC_MAINPAGE_H
#define DIVEC_MAINPAGE_H

#include <display/pages/Page.h>

#include <display/widgets/NumberValueWidget.h>
#include <display/widgets/StringValueWidget.h>
#include <display/widgets/BatteryWidget.h>

#define TOP_ROW_Y 1
#define MIDDLE_ROW_Y 85
#define BOTTOM_ROW_Y 170

class MainPage : public Page {
public:
    MainPage(Display *display, DecoManager *decoManager);

    ~MainPage() override;

protected:
    void update() override;

    void redraw() override;

    uint8_t handleRightButtonPress() override;


private:
    NumberValueWidget *_currentDepth;
    NumberValueWidget *_diveTime;
    NumberValueWidget *_surfaceInterval;
    NumberValueWidget *_decoStopDepth;
    NumberValueWidget *_decoStopTime;

    NumberValueWidget *_gasPO2;

    StringValueWidget *_mode;
    StringValueWidget *_gasMix;
    NumberValueWidget *_ndl;
    NumberValueWidget *_tts;

    BatteryWidget *_battery;
};


#endif //DIVEC_MAINPAGE_H
