#ifndef DIVEC_ENCODEROKBUTTONLISTENER_H
#define DIVEC_ENCODEROKBUTTONLISTENER_H

#include <SwitchInput.h>


class EncoderOkButtonListener : public SwitchListener {
public:

    virtual void okButtonClicked(bool held) {
        // overide me
    }

    void onPressed(uint8_t /*pin*/, bool held) override {
        // do nothing yet - ignore and wait for release
    }

    void onReleased(uint8_t /*pin*/, bool held) override {
        menuMgr.onMenuSelect(held);
        okButtonClicked(held);
    }
};


#endif //DIVEC_ENCODEROKBUTTONLISTENER_H
