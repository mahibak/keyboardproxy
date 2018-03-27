#pragma once

#include <Callbacks.h>
#include "ps2.h"

class Ps2Keyboard: CallbackListener<const PS2::ByteReceivedArgs*>
{
public:
    enum class ScanCode
    {
        ESC = 0x76,
        F1 = 0x5,
        F2 = 0x6,
        F3 = 0x4,
        F4 = 0x0C,
        F5 = 0x3,
        F6 = 0x0B,
        F7 = 0x83,
        F8 = 0x0A,
        F9 = 0x1,
        F10 = 0x9,
        F11 = 0x78,
        F12 = 0x7,
        //PrtScr = 0xE012E07C,
        ScrollLock = 0x7E,
        //PauseBreak = 0xE11477E1F014E077,
        Pound = 0x0E,
        Top1 = 0x16,
        Top2 = 0x1E,
        Top3 = 0x26,
        Top4 = 0x25,
        Top5 = 0x2E,
        Top6 = 0x36,
        Top7 = 0x3D,
        Top8 = 0x3E,
        Top9 = 0x46,
        Top0 = 0x45,
        TopMinus = 0x4E,
        TopEqual = 0x55,
        Backspace = 0x66,
        Tab = 0x0D,
        Q = 0x15,
        W = 0x1D,
        E = 0x24,
        R = 0x2D,
        T = 0x2C,
        Y = 0x35,
        U = 0x3C,
        I = 0x43,
        O = 0x44,
        P = 0x4D,
        SquareBracketOpen = 0x54,
        SquareBracketClose = 0x5B,
        Backslash = 0x5D,
        CapsLock = 0x58,
        A = 0x1C,
        S = 0x1B,
        D = 0x23,
        F = 0x2B,
        G = 0x34,
        H = 0x33,
        J = 0x3B,
        K = 0x42,
        L = 0x4B,
        SemiColon = 0x4C,
        Aprostrophe = 0x52,
        Enter = 0x5A,
        ShiftLeft = 0x12,
        Z = 0x1A,
        X = 0x22,
        C = 0x21,
        V = 0x2A,
        B = 0x32,
        N = 0x31,
        M = 0x3A,
        Colon = 0x41,
        Period = 0x49,
        Slack = 0x4A,
        ShiftRight = 0x59,
        CtrlLeft = 0x14,
        WindowsLeft = 0xE01F,
        AltLeft = 0x11,
        Spacebar = 0x29,
        AltRight = 0xE011,
        WindowsRight = 0xE027,
        Menus = 0xE02F,
        CtrlRight = 0xE014,
        Insert = 0xE070,
        Home = 0xE06C,
        PageUp = 0xE07D,
        Delete = 0xE071,
        End = 0xE069,
        PageDown = 0xE07A,
        UpArrow = 0xE075,
        LeftArrow = 0xE06B,
        DownArrow = 0xE072,
        RightArrow = 0xE074,
        NumLock = 0x77,
        KeypadSlash = 0xE04A,
        KeypadStar = 0x7C,
        KeypadMinus = 0x7B,
        Keypad7 = 0x6C,
        Keypad8 = 0x75,
        Keypad9 = 0x7D,
        KeypadPlus = 0x79,
        Keypad4 = 0x6B,
        Keypad5 = 0x73,
        Keypad6 = 0x74,
        Keypad1 = 0x69,
        Keypad2 = 0x72,
        Keypad3 = 0x7A,
        Keypad0 = 0x70,
        KeypadPeriod = 0x71,
        KeypadEnter = 0xE05A,
    };

public:
    Ps2Keyboard(PS2* ps2)
    {
        ps2->onByteReceived.addListener(this);
    }

    bool released = false;
    bool extended = false;

    class KeyArgs
    {
public:
        ScanCode scanCode;
    };

    CallbackEmitter<const KeyArgs*> onKeyDown;
    CallbackEmitter<const KeyArgs*> onKeyUp;
    CallbackEmitter<const KeyArgs*> onKeyPressed;

    void onCallback(const PS2::ByteReceivedArgs* args) override
    {
        if(args->byte == 0xF0)
            released = true;
        else if(args->byte == 0xE0)
            extended = true;
        else
        {
            KeyArgs a;
            a.scanCode = static_cast<ScanCode>((extended ? 0xE000 : 0) | args->byte);
            if(released)
                onKeyUp.emit(&a);
            else
                onKeyDown.emit(&a);

            released = false;
            extended = false;
        }
    }
};
