#ifndef DIVEC_ENCODERNEXTOKBUTTON_H
#define DIVEC_ENCODERNEXTOKBUTTON_H


#include <tcMenu.h>
#include <SwitchInput.h>
#include "EncoderOkButtonListener.h"
#include "EncoderNextButtonListener.h"

/**
 * An emulation of a rotary encoder using only a "Next" switch instead of "Up/Down"
 */
class EncoderNextOkButton : public RotaryEncoder {
public:

    // registering an okKeyListener is not really needed while using a CustomDrawing as the renderloop allready contains the status of this button.
    static void setupNextOkButtonEncoder(pinid_t pinNext, pinid_t pinOk, EncoderNextButtonListener *nextKeyListener = nullptr, EncoderOkButtonListener *okKeyListener = nullptr);

private:
    EncoderNextOkButton(pinid_t pinNext, pinid_t pinOk, EncoderNextButtonListener *nextKeyCallback, EncoderOkButtonListener *okKeyListener);

    static uint8_t getMaxMenuItemIndex(MenuItem *root);

    static void onValueChanged(int value);
};


#endif //DIVEC_ENCODERNEXTOKBUTTON_H
