#pragma once

#include <key.h>
#include <sys/_stdint.h>
#include <sys/cdefs.h>
#include <usbd_def.h>
#include <usbd_hid.h>

class UsbKeyboard
{
    uint8_t scanCodes[104] = { 0x29, //ESC
            0x3a, //F1
            0x3b, //F2
            0x3c, //F3
            0x3d, //F4
            0x3e, //F5
            0x3f, //F6
            0x40, //F7
            0x41, //F8
            0x42, //F9
            0x43, //F10
            0x44, //F11
            0x45, //F12
            0x47, //ScrollLock
            0x35, //Pound
            0x1e, //Top1
            0x1f, //Top2
            0x20, //Top3
            0x21, //Top4
            0x22, //Top5
            0x23, //Top6
            0x24, //Top7
            0x25, //Top8
            0x26, //Top9
            0x27, //Top0
            0x2d, //TopMinus
            0x2e, //TopEqual
            0x2a, //Backspace
            0x2b, //Tab
            0x14, //Q
            0x1a, //W
            0x08, //E
            0x15, //R
            0x17, //T
            0x1c, //Y
            0x18, //U
            0x0c, //I
            0x12, //O
            0x13, //P
            0x2f, //SquareBracketOpen
            0x30, //SquareBracketClose
            0x31, //Backslash
            0x39, //CapsLock
            0x04, //A
            0x16, //S
            0x07, //D
            0x09, //F
            0x0a, //G
            0x0b, //H
            0x0d, //J
            0x0e, //K
            0x0f, //L
            0x33, //SemiColon
            0x34, //Aprostrophe
            0x28, //Enter
            0xe1, //ShiftLeft
            0x1d, //Z
            0x1b, //X
            0x06, //C
            0x19, //V
            0x05, //B
            0x11, //N
            0x10, //M
            0x36, //Comma
            0x37, //Period
            0x38, //Slash
            0xe5, //ShiftRight
            0xe0, //CtrlLeft
            0xe3, //WindowsLeft
            0xe2, //AltLeft
            0x2c, //Spacebar
            0xe6, //AltRight
            0xe7, //WindowsRight
            0x65, //Menus
            0xe4, //CtrlRight
            0x49, //Insert
            0x4a, //Home
            0x4b, //PageUp
            0x4c, //Delete
            0x4d, //End
            0x4e, //PageDown
            0x52, //UpArrow
            0x50, //LeftArrow
            0x51, //DownArrow
            0x4f, //RightArrow
            0x53, //NumLock
            0x54, //KeypadSlash
            0x55, //KeypadStar
            0x56, //KeypadMinus
            0x5f, //Keypad7
            0x60, //Keypad8
            0x61, //Keypad9
            0x57, //KeypadPlus
            0x5c, //Keypad4
            0x5d, //Keypad5
            0x5e, //Keypad6
            0x59, //Keypad1
            0x5a, //Keypad2
            0x5b, //Keypad3
            0x62, //Keypad0
            0x63, //KeypadPeriod
            0x58, //KeypadEnter
            0x48, //PauseBreak
            0x46, //PrtScr
            };

    class KeyboardReport
    {
    public:
        uint8_t modifiers = 0;
        uint8_t reserved = 0;
        uint8_t keys[6];

        void send()
        {
            extern USBD_HandleTypeDef hUsbDeviceFS;
            USBD_HID_SendReport(&hUsbDeviceFS, (uint8_t*)this, 8);
            while (((USBD_HID_HandleTypeDef*)hUsbDeviceFS.pClassData)->state == HID_BUSY)
            {

            }
        }

        void clear()
        {
            modifiers = 0;
            reserved = 0;
            keys[0] = 0;
            keys[1] = 0;
            keys[2] = 0;
            keys[3] = 0;
            keys[4] = 0;
            keys[5] = 0;
        }
    }__packed;
    static KeyboardReport report;

public:
    void releaseKey(Key key)
    {
        report.clear();
        report.send();
    }

    void pressKey(Key key)
    {
        report.clear();
        report.keys[0] = scanCodes[static_cast<uint8_t>(key)];
        report.send();
    }
};
