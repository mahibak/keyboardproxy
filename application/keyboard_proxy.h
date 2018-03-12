#pragma once

#include <HAL/Led.h>
#include <main.h>
#include <scan_codes.h>
#include <stm32f1xx_hal.h>
#include <stdint.h>
#include <Scheduling/Scheduler.h>
#include <STM32/Stm32HalClock.h>
#include <STM32/Stm32Led.h>
#include <STM32/Stm32Processor.h>
#include <usbd_def.h>
#include <usbd_hid.h>

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

class KeyboardProxy
{
    void pressKey(uint8_t scanCode)
    {
        KeyboardReport report;
        report.clear();
        report.keys[0] = 4;
        report.send();
        HAL_Delay(100);
        report.clear();
        report.send();
    }

public:
    KeyboardProxy()
    {
        static Stm32HalClock stm32HalClock;
        Clock::monotonic = &stm32HalClock;
        static Scheduler scheduler;
        static Stm32Processor stm32Processor;
        Processor::instance = &stm32Processor;
        static Stm32Led led(LED_GPIO_Port, LED_Pin);
        static SoftwarePwmLed softwarePwmLed(&led, 100);
        scheduler.spawnProcess(&softwarePwmLed);


        HAL_Delay(10000);
        while (true)
        {
            pressKey(KEY_D);
            Clock::monotonic->delayMilliseconds(1000);
        }

        scheduler.runInfinitely();
    }
};
