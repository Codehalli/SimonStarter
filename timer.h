/*
 * timer.h
 *
 *  Created on: December 10, 2019
 *      Author: Pranav Rao
 */

#ifndef TIMER_H_
#define TIMER_H_

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

typedef struct
{
    uint32_t timerID;            // e.g., TIMER32_0_BASE
    uint32_t sourceClockF_inMHz; // e.g., 48
    uint32_t prescalar;          // e.g., TIMER32_PRESCALER_1
    uint32_t loadValue;          // e.g., 0xffffffff
    uint32_t rolloverCount;      // contains # of roll overs
    uint32_t interruptNumber;    // (more about this later)
} HWTimer_t;

typedef struct
{
    HWTimer_t   *hwtimer_p;  // HW timer used for this SW timer
    uint32_t    waitCycles;  // number of HW counts to wait
    uint32_t    startCounter;      // HW counter value at start
    uint32_t    startRolloverCount;// HW roll over value at start
} OneShotSWTimer_t;

extern HWTimer_t timer0, timer1;

#define SYS_CLOCK_F_MHz 3.5

void initHWTimer0();
void StartOneShotSWTimer(OneShotSWTimer_t *OST_p);
void InitOneShotSWTimer(OneShotSWTimer_t *OST_p,
                        HWTimer_t *hwtimer_p,
                        uint32_t  waitTime_us);
bool OneShotSWTimerExpired(OneShotSWTimer_t* OST_p);
void startHWTimer(HWTimer_t hwTimer);
bool Timer0_Expired(void);
uint32_t WaitCycles(HWTimer_t *hwtimer_p,  uint32_t  waitTime_us);
void Debounce_Button(int msec);

void splashClock(int usec);



#endif /* TIMER_H_ */
