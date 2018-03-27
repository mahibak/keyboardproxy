#pragma once

#include <Callbacks.h>
#include <Containers/InplaceQueue.h>
#include <key.h>
#include <ps2.h>
#include <stdint.h>

class Ps2Keyboard: CallbackListener<const PS2::ByteReceivedArgs*>
{
    //PauseBreak0xE11477E1F014E077
    //PrtScr0xE012E07C

public:
    uint16_t lookUpTable[102] = { 0x76, 0x5, 0x6, 0x4, 0x0C, 0x3, 0x0B, 0x83, 0x0A, 0x1, 0x9, 0x78, 0x7, 0x7E, 0x0E, 0x16, 0x1E, 0x26, 0x25, 0x2E, 0x36, 0x3D, 0x3E, 0x46, 0x45, 0x4E, 0x55, 0x66, 0x0D, 0x15, 0x1D, 0x24, 0x2D, 0x2C, 0x35, 0x3C, 0x43, 0x44, 0x4D, 0x54, 0x5B, 0x5D, 0x58, 0x1C, 0x1B, 0x23, 0x2B, 0x34, 0x33, 0x3B, 0x42, 0x4B, 0x4C, 0x52, 0x5A, 0x12, 0x1A, 0x22, 0x21, 0x2A, 0x32, 0x31, 0x3A, 0x41, 0x49, 0x4A, 0x59, 0x14, 0xE01F, 0x11, 0x29, 0xE011, 0xE027, 0xE02F, 0xE014, 0xE070, 0xE06C, 0xE07D, 0xE071, 0xE069, 0xE07A, 0xE075, 0xE06B, 0xE072, 0xE074, 0x77, 0xE04A, 0x7C, 0x7B, 0x6C, 0x75, 0x7D, 0x79, 0x6B, 0x73, 0x74, 0x69, 0x72, 0x7A, 0x70, 0x71, 0xE05A };

public:
    Ps2Keyboard(PS2* ps2)
    {
        ps2->onByteReceived.addListener(this);
    }

    bool released = false;
    bool extended = false;

    class KeyDownArgs
    {
    public:
        Key key;
    };
    class KeyUpArgs
    {
    public:
        Key key;
    };

    CallbackEmitter<const KeyDownArgs*> onKeyDown;
    CallbackEmitter<const KeyUpArgs*> onKeyUp;

    Key getKey(uint16_t pressedKey)
    {
        for (uint32_t i = 0; i < sizeof(lookUpTable) / sizeof(lookUpTable[0]); i++)
        {
            if (lookUpTable[i] == pressedKey)
            {
                return static_cast<Key>(i);
            }
        }
    }

    void onCallback(const PS2::ByteReceivedArgs* args) override
    {
        if (args->byte == 0xF0)
            released = true;
        else if (args->byte == 0xE0)
            extended = true;
        else
        {
            if (released)
            {
                KeyUpArgs a;
                a.key = getKey((extended ? 0xE000 : 0) | args->byte);
                onKeyUp.emit(&a);
            }
            else
            {
                KeyDownArgs a;
                a.key = getKey((extended ? 0xE000 : 0) | args->byte);
                onKeyDown.emit(&a);
            }

            released = false;
            extended = false;
        }
    }
};
