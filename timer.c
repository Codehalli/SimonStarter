/*
 *  timer.c
 *
 *  Created on: December 10, 2019
 *      Author: Pranav Rao
 */

#include "timer.h"
#include "stdio.h"

HWTimer_t timer0;
void initHWTimer0()
{
    timer0.loadValue = 0xffffffff;//0xffffffff;
    timer0.prescalar = 1;      // desired numeric prescale value
    timer0.rolloverCount = 0;  // initialize this running count to 0
    timer0.sourceClockF_inMHz = SYS_CLOCK_F_MHz; // 48
    timer0.timerID = TIMER32_0_BASE;             // specify Timer32 0
    timer0.interruptNumber = INT_T32_INT1;

    startHWTimer(timer0);
}

void startHWTimer(HWTimer_t hwTimer)
{
   // ...
   Timer32_initModule(hwTimer.timerID,
                      TIMER32_PRESCALER_1, //  TIMER32_PRESCALER_1
                      TIMER32_32BIT, // 32-bit mode
                      TIMER32_PERIODIC_MODE);

   // TODO: We will discuss the interrupt-related parts later.
   //Interrupt_enableInterrupt(hwTimer.interruptNumber);

   // Set the load value and start the HW timer
   Timer32_setCount(hwTimer.timerID, hwTimer.loadValue);

   // (false indicates "This is not one-shot mode�)
   Timer32_startTimer(hwTimer.timerID, false);
}

void InitOneShotSWTimer(OneShotSWTimer_t *OST_p,
                        HWTimer_t *hwtimer_p,
                        uint32_t  waitTime_us)
{
    OST_p->hwtimer_p = hwtimer_p;
    OST_p->waitCycles = WaitCycles(OST_p->hwtimer_p, waitTime_us);
}

void StartOneShotSWTimer(OneShotSWTimer_t *OST_p)
{
    // Similar to you looking at your watch
    //  and recording the current time.
    //  Stores current Timer32 value as well as
    //  number of rollovers so far.
    OST_p->startCounter =Timer32_getValue((OST_p->hwtimer_p)->timerID);
    OST_p->startRolloverCount = OST_p->hwtimer_p->rolloverCount;
}

bool OneShotSWTimerExpired(OneShotSWTimer_t* OST_p)
{
    bool Timer0_Expired();
    bool expired = false;

    // Compute HW timer period, in units of timer32 counts
    uint64_t hwPeriodInCycle = (uint64_t) OST_p->hwtimer_p->loadValue+ (uint64_t) 1;

    // Get HW timer�s current value
    uint64_t currentCounter = Timer32_getValue((OST_p->hwtimer_p)->timerID);

    // Get HW timer�s �start� value
    uint64_t startCounter = OST_p->startCounter;
    // Compute total number of rollover events
    uint32_t rollovers = OST_p->hwtimer_p->rolloverCount
                       - OST_p->startRolloverCount;
    // Compute total number of HW timer counts that have occurred since starting
    uint64_t rolloverCycles = (uint64_t) rollovers * (uint64_t) hwPeriodInCycle;
    uint64_t ElapsedCycles = rolloverCycles + startCounter - currentCounter;

    if (ElapsedCycles >= OST_p->waitCycles)
        expired = true;
    else
        expired = false;
    return expired;
}
bool Timer0_Expired()
{
    // TODO: Check Timer0 for completion.
    // - A function that accomplishes this task is in the lecture slides. You will need to explain why it works.
    static unsigned int previousSnap = 0;//a static local is initialized once and then retains it's value in future calls
    unsigned int currentSnap;
    bool returnValue=false;

    currentSnap = Timer32_getValue(timer0.timerID);//gets the count
    //see if the count has reset to 10000
    if (currentSnap > previousSnap)//this makes it easier to understand what is happening and allows for a nice break point to check the vals of current and previous
    {
        returnValue =true ;
        timer0.rolloverCount++;
    }
    previousSnap = currentSnap;//save the count for next time to see if the timer has expired
    return returnValue;

}
uint32_t WaitCycles(HWTimer_t *hwtimer_p,  uint32_t  waitTime_us)
{
    int cyc= hwtimer_p->sourceClockF_inMHz*waitTime_us;
    return cyc;
}

void Debounce_Button(int msec)
{
    OneShotSWTimer_t newTimer;
    InitOneShotSWTimer(&newTimer, &timer0, msec);
    StartOneShotSWTimer(&newTimer);
    while (!OneShotSWTimerExpired(&newTimer))
    {

    }
}

void splashClock(int usec)
{
    OneShotSWTimer_t newTimer;
    InitOneShotSWTimer(&newTimer, &timer0, usec);
    StartOneShotSWTimer(&newTimer);
    while (!OneShotSWTimerExpired(&newTimer))
    {

    }
}
