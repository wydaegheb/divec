#ifndef DIVEC_SETTINGSPAGE_H
#define DIVEC_SETTINGSPAGE_H

#include <display/pages/Page.h>

#include <display/widgets/NumberValueWidget.h>
#include <display/widgets/EditableDigitWidget.h>
#include <display/widgets/StringValueWidget.h>
#include <display/widgets/BatteryWidget.h>


class SettingsPage : public Page {
public:
    SettingsPage(Display *display, DecoManager *decoManager);

    ~SettingsPage() override;

protected:
    void update() override;

    void redraw() override;

    uint8_t handleLeftButtonPress() override;

    uint8_t handleRightButtonPress() override;


private:
    EditableDigitWidget *_testNr;

};


#endif //DIVEC_SETTINGSPAGE_H
