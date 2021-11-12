#include "EncoderNextOkButton.h"

void EncoderNextOkButton::setupNextOkButtonEncoder(pinid_t pinNext, pinid_t pinOk,
                                                   EncoderNextButtonListener *nextKeyListener,
                                                   EncoderOkButtonListener *okKeyListener) {
    switches.setEncoder(new EncoderNextOkButton(pinNext, pinOk,
                                                (nextKeyListener == nullptr ? new EncoderNextButtonListener()
                                                                            : nextKeyListener),
                                                (okKeyListener == nullptr ? new EncoderOkButtonListener()
                                                                          : okKeyListener)));
}

EncoderNextOkButton::EncoderNextOkButton(pinid_t pinNext, pinid_t pinOk, EncoderNextButtonListener *nextKeyListener,
                                         EncoderOkButtonListener *okKeyListener) : RotaryEncoder(
        &EncoderNextOkButton::onValueChanged) {
    switches.initialiseInterrupt(internalDigitalIo(), true);
    switches.addSwitchListener(pinOk, okKeyListener);
    switches.addSwitchListener(pinNext, nextKeyListener);
    switches.setEncoder(this);
}


void EncoderNextOkButton::onValueChanged(int value) { // implements "wrap around behaviour" for the "Next" button
    int maxValue;
    MenuItem *currentEditedMenuItem = menuMgr.getCurrentEditor();
    if (currentEditedMenuItem) {
        if (isMenuRuntimeMultiEdit(
                currentEditedMenuItem)) { // multi part edit -> get max from the range currently being edited
            maxValue = reinterpret_cast<EditableMultiPartMenuItem<void *> *>(currentEditedMenuItem)->getCurrentRange();
        } else {  // simple edit -> get max from the item
            maxValue = currentEditedMenuItem->getMaximumValue();
        }
    } else { // walking over menu items -> max is number of items in the current menu
        maxValue = getMaxMenuItemIndex(getParentRoot(menuMgr.getCurrentMenu()));
    }

    if (value == maxValue) { // wrap around when max is reached
        // we are setting the current value of the encoder to -1 as the next call will increment the current reading before calling this function again (otherwise we would start at 1 iso of 0)
        switches.getEncoder()->setCurrentReading(-1);
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


