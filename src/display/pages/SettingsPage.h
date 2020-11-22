#ifndef DIVEC_SETTINGSPAGE_H
#define DIVEC_SETTINGSPAGE_H

#include "Page.h"

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

    char const *handleLeftButtonPress() override;


private:
    EditableDigitWidget *_testNr;

};


#endif //DIVEC_SETTINGSPAGE_H
