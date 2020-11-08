#ifndef DIVEC_SETTINGSPAGE_H
#define DIVEC_SETTINGSPAGE_H

#include "Page.h"

#include <display/widgets/NumberValueWidget.h>
#include <display/widgets/StringValueWidget.h>
#include <display/widgets/BatteryWidget.h>


class SettingsPage : public Page {
public:
    SettingsPage(Display *display, DecoManager *decoManager);

    ~SettingsPage() override;

protected:
    void update() override;

private:

};


#endif //DIVEC_SETTINGSPAGE_H
