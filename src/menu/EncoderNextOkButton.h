#ifndef DIVEC_ENCODERNEXTOKBUTTON_H
#define DIVEC_ENCODERNEXTOKBUTTON_H


#include <tcMenu.h>
#include <SwitchInput.h>

/**
 * An emulation of a rotary encoder using only a "Next" switch instead of "Up/Down"
 */
class EncoderNextOkButton : public RotaryEncoder {
public:
    static void setupNextOkButtonEncoder(pinid_t pinNext, pinid_t pinOk);

    static void setMinYear(uint16_t minYear);

    static void setMaxYear(uint16_t maxYear);

private:
    EncoderNextOkButton(pinid_t pinNext, pinid_t pinOk, KeyCallbackFn keyCallbackFn, EncoderCallbackFn callback);

    static uint8_t getMaxMenuItemIndex(MenuItem *root);

    static void onOkButtonPress(pinid_t key, bool heldDown);

    static void onNextButtonPress(pinid_t key, bool heldDown);

    static void onValueChanged(int value);

private:
    static uint16_t MIN_YEAR;
    static uint16_t MAX_YEAR;
};


#endif //DIVEC_ENCODERNEXTOKBUTTON_H
