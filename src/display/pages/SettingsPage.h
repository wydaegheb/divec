#ifndef DIVEC_SETTINGSPAGE_H
#define DIVEC_SETTINGSPAGE_H

#include <display/pages/Page.h>

#include <display/widgets/NumberValueWidget.h>
#include <display/widgets/EditableDigitWidget.h>
#include <display/widgets/StringValueWidget.h>
#include <display/widgets/BatteryWidget.h>


class SettingsPage final : public Page {
public:
    SettingsPage(Display *display, DecoManager *decoManager);

    ~SettingsPage() final;

protected:
    void update() final;

    void redraw() final;

    uint8_t handleLeftButtonPress() final;

    uint8_t handleRightButtonPress() final;


private:
    EditableDigitWidget *_testNr;

};


#endif //DIVEC_SETTINGSPAGE_H
