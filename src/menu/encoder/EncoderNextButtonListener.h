#ifndef DIVEC_ENCODERNEXTBUTTONLISTENER_H
#define DIVEC_ENCODERNEXTBUTTONLISTENER_H

#include <SwitchInput.h>
#include <tcMenu.h>


class EncoderNextButtonListener : public SwitchListener {
public:

    virtual void nextButtonClicked(bool held) {
        // override me
    }

    void onPressed(uint8_t /*pin*/, bool held) override {
        // do nothing yet - ignore and wait for release
    }

    void onReleased(uint8_t /*pin*/, bool held) override {
        switches.getEncoder()->increment(1);
        nextButtonClicked(held);
    }
};


#endif //DIVEC_ENCODERNEXTBUTTONLISTENER_H
