#pragma once

#include <HAL/Led.h>
#include <main.h>
#include <mapping.h>
#include <ps2.h>
#include <ps2_keyboard.h>
#include <stm32f103xb.h>
#include <stm32f1xx_hal_gpio.h>
#include <Scheduling/Scheduler.h>
#include <STM32/stm32_pin.h>
#include <STM32/Stm32Led.h>
#include <STM32/Stm32Processor.h>
#include <STM32/Stm32TimerClock.h>
#include <tim.h>
#include <Time/Clock.h>
#include <usb_keyboard.h>


class KeyboardProxy
{

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
//        scheduler.spawnProcess(&softwarePwmLed);

        static Stm32Pin clock(GPIOB, GPIO_PIN_4);
        static Stm32Pin data(GPIOB, GPIO_PIN_6);

        static PS2 ps2(&clock, &data);
        static Ps2Keyboard ps2Keyboard(&ps2);
        static UsbKeyboard usbKeyboard;
        static Mapping mapping(&usbKeyboard, &ps2Keyboard);
        scheduler.spawnProcess(&ps2);

        scheduler.runInfinitely();
    }
};
