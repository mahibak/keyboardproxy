#pragma once

#include <Callbacks.h>
#include <ps2_keyboard.h>
#include <usb_keyboard.h>

class Mapping: CallbackListener<const Ps2Keyboard::KeyDownArgs*>, CallbackListener<const Ps2Keyboard::KeyUpArgs*>
{
    UsbKeyboard* usbKeyboard;
    Ps2Keyboard* ps2Keyboard;

public:
    Mapping(UsbKeyboard* usbKeyboard, Ps2Keyboard* ps2Keyboard) :
            usbKeyboard(usbKeyboard),
            ps2Keyboard(ps2Keyboard)
    {
        ps2Keyboard->onKeyDown.addListener(this);
        ps2Keyboard->onKeyUp.addListener(this);
    }

    void onCallback(const Ps2Keyboard::KeyDownArgs* args) override
    {
        usbKeyboard->pressKey(args->key);
    }

    void onCallback(const Ps2Keyboard::KeyUpArgs* args) override
    {
        usbKeyboard->releaseKey(args->key);
    }
};
