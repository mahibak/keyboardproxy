class OsTimerTicker: public Process
{
public:
    DoublyLinkedList<OsTimer> timers;

    void loop() override
    {
        uint64_t now = Clock::monotonic->getTicks();
        OsTimer* timer;
        while ((timer = timers.getFirst()) != nullptr && timer->tick <= now)
        {
            timer->callbackee->onCallback(timer);
            timers.remove(timer);
            now = Clock::monotonic->getTicks();
        }
    }

    void start(OsTimer* timer)
    {
        OsTimer* iterator = timers.next(nullptr);

        if(iterator == nullptr)
            timers.add(timer);
        else
        {
            if(iterator->)
        }
    }
};

class OsTimer: public DoublyLinkedListElement<OsTimer>
{
public:
    uint64_t tick = 0;
    uint64_t periodTicks = 0;
    CallbackListener<OsTimer*>* callbackee = nullptr;
    OsTimerTicker* osTimerTicker;

public:
    OsTimer(CallbackEmitter<OsTimer*>* callbackee, OsTimerTicker* osTimerTicker) :
            callbackee(callbackee),
            osTimerTicker(osTimerTicker)
    {

    }

    void startMicroseconds(uint64_t microseconds)
    {
        periodTicks = Clock::monotonic->microsecondsToTicks(microseconds);
        osTimerTicker->start(this);
    }
};
