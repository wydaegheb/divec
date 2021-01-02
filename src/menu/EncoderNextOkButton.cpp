#include "EncoderNextOkButton.h"

uint16_t EncoderNextOkButton::MIN_YEAR = 2020;
uint16_t EncoderNextOkButton::MAX_YEAR = 2100;

EncoderNextOkButton::EncoderNextOkButton(pinid_t pinNext, pinid_t pinOk, KeyCallbackFn keyCallbackFn, EncoderCallbackFn callback) : RotaryEncoder(callback) {
    switches.initialiseInterrupt(internalDigitalIo(), true);
    switches.addSwitch(pinOk, nullptr);
    switches.onRelease(pinOk, &EncoderNextOkButton::onOkButtonPress);
    switches.addSwitch(pinNext, keyCallbackFn);
    switches.setEncoder(this);

}

void EncoderNextOkButton::setupNextOkButtonEncoder(pinid_t pinNext, pinid_t pinOk) {
    switches.setEncoder(new EncoderNextOkButton(pinNext, pinOk, &EncoderNextOkButton::onNextButtonPress, &EncoderNextOkButton::onValueChanged));
}

void EncoderNextOkButton::onOkButtonPress(pinid_t key, bool heldDown) {
    menuMgr.onMenuSelect(heldDown);
}

void EncoderNextOkButton::onNextButtonPress(pinid_t key, bool heldDown) {
    switches.getEncoder()->increment(1);
}

void EncoderNextOkButton::onValueChanged(int value) { // implements "wrap around behaviour" for the "Next" button
    int minValue = 0;
    int maxValue;
    MenuItem *currentEditedMenuItem = menuMgr.getCurrentEditor();
    if (currentEditedMenuItem) {
        if (isMenuRuntimeMultiEdit(currentEditedMenuItem)) { // multi part edit
            maxValue = reinterpret_cast<EditableMultiPartMenuItem<void *> *>(currentEditedMenuItem)->getCurrentRange();

            // special treatment is needed for dates. wrap around should start at 1 instead of 0 (month 0 or day 0 doesn't exist) and to avoid wrapping the year from 0 -> 9999 the year is limited from MIN_DATE_YEAR -> MAX_DATE_YEAR
            // (a better solution would be that tcMenu supports a date where every digit of the year can be edited instead of the full year at once)
            if (currentEditedMenuItem->getMenuType() == MENUTYPE_DATE) {
                if (value > MIN_YEAR) { // custom wrap around date year (otherwise year goes from 0 -> 9999 which is just to many clicks ;-))
                    minValue = MIN_YEAR;
                    maxValue = MAX_YEAR;
                } else {
                    minValue = 1; // for days and months
                }
            }

        } else {  // simple edit -> get max from the item
            maxValue = currentEditedMenuItem->getMaximumValue();
        }
    } else { // walking over menu items -> max is number of items in the current menu
        maxValue = getMaxMenuItemIndex(getParentRoot(menuMgr.getCurrentMenu()));
    }

    if (value == maxValue) { // wrap around when max is reached
        // we are setting the current value of the encoder to (minvalue - 1) as the next call will first increment before calling this function
        switches.getEncoder()->setCurrentReading(minValue - 1);
    }
    menuMgr.valueChanged(value);

}

uint8_t EncoderNextOkButton::getMaxMenuItemIndex(MenuItem *root) {
    uint8_t count = 0;
    MenuItem *item = root;
    while (item) {
        count++;
        item = item->getNext();
    }
    return (count - 1); // 0 based -> highest index is amount - 1
}

void EncoderNextOkButton::setMinYear(uint16_t minYear) {
    MIN_YEAR = minYear;
}

void EncoderNextOkButton::setMaxYear(uint16_t maxYear) {
    MAX_YEAR = maxYear;
}
