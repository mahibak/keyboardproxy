#pragma once

#include <Callbacks.h>
#include <Containers/InplaceQueue.h>
#include <stdint.h>
#include <Scheduling/Process.h>
#include <STM32/stm32_pin.h>
#include <Time/Clock.h>

class PS2: CallbackListener<Stm32Pin::ExternalInterruptArgs*>, public Process
{
private:
    uint8_t clockEdges = 0;
    bool startBit = false;
    bool parity = false;
    bool stopBit = false;
    uint8_t dataByte = 0;

    Stm32Pin* clock;
    Stm32Pin* data;

public:
    PS2(Stm32Pin* clock, Stm32Pin* data) :
            clock(clock),
            data(data)
    {
        clock->setOpenDrainHighPullUp();
        data->setOpenDrainHighPullUp();
        clock->addListener(this);
    }

    class ByteReceivedArgs
    {
    public:
        uint8_t byte;
    };

    CallbackEmitter<const ByteReceivedArgs*> onByteReceived;

    uint64_t us = 0;

    InplaceQueue<uint8_t, 128> rxQueue;

    void loop() override
    {
        ByteReceivedArgs a;

        while(!rxQueue.isEmpty())
        {
            a.byte = rxQueue.pull();
            onByteReceived.emit(&a);
        }
    }

    void onCallback(Stm32Pin::ExternalInterruptArgs* args) override
    {
        uint64_t now = Clock::monotonic->microseconds();
        if ((now - us) > 1000)
            clockEdges = 0;
        us = now;

        switch (clockEdges++)
        {
        case 0:
            startBit = data->high();
            break;
        case 1:
            dataByte |= data->high() ? (1 << 0) : 0;
            break;
        case 2:
            dataByte |= data->high() ? (1 << 1) : 0;
            break;
        case 3:
            dataByte |= data->high() ? (1 << 2) : 0;
            break;
        case 4:
            dataByte |= data->high() ? (1 << 3) : 0;
            break;
        case 5:
            dataByte |= data->high() ? (1 << 4) : 0;
            break;
        case 6:
            dataByte |= data->high() ? (1 << 5) : 0;
            break;
        case 7:
            dataByte |= data->high() ? (1 << 6) : 0;
            break;
        case 8:
            dataByte |= data->high() ? (1 << 7) : 0;
            break;
        case 9:
            parity = data->high();
            break;
        case 10:
            stopBit = data->high();
            clockEdges = 0;
            if (stopBit == 1 && startBit == 0)
                rxQueue.push(dataByte);
            dataByte = 0;
            break;
        default:
            clockEdges = 0;
            break;
        }
    }
};
