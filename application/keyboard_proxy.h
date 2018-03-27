#pragma once

#include <Callbacks.h>
#include <HAL/Led.h>
#include <main.h>
#include <STM32/stm32f_hal.h>
#include <stm32f1xx_hal_gpio.h>
#include <stdint.h>
#include <Scheduling/Scheduler.h>
#include <STM32/stm32_pin.h>
#include <STM32/Stm32Led.h>
#include <STM32/Stm32Processor.h>
#include <STM32/Stm32TimerClock.h>
#include <tim.h>
#include <Time/Clock.h>
#include <usbd_def.h>
#include <usbd_hid.h>

#include "../Src/ps2.h"
#include "../Src/ps2_keyboard.h"

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

class KeyboardProxy
{
    void pressKey(uint8_t scanCode)
    {
        report.clear();
        report.keys[0] = scanCode;
        report.send();
        report.clear();
        report.send();
    }

public:
    KeyboardProxy()
    {
        static Stm32TimerClock stm32HalClock(&htim1);
        Clock::monotonic = &stm32HalClock;
        static Scheduler scheduler;
        static Stm32Processor stm32Processor;
        Processor::instance = &stm32Processor;
        static Stm32Led led(LED_GPIO_Port, LED_Pin);
        static SoftwarePwmLed softwarePwmLed(&led, 100);
        scheduler.spawnProcess(&softwarePwmLed);

        static Stm32Pin clock(GPIOB, GPIO_PIN_4);
        static Stm32Pin data(GPIOB, GPIO_PIN_6);

        static PS2 ps2(&clock, &data);
        static Ps2Keyboard ps2Keyboard(&ps2);

        while (true)
        {
            //pressKey(KEY_D);
            Clock::monotonic->delayMilliseconds(1000);
        }

        scheduler.runInfinitely();
    }
};
