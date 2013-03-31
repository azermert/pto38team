#ifndef _TIMEBASE_H
#define _TIMEBASE_H

#include "misc.h"

#define timeStep 40	//microseconds

/* Accesible function prototypes */
void init_TimeBase(void);

void delay(u32 time);			//time delay, wait specified amount of time
void waitOnTime(u32 trgtTime);	//wait until target time is reached
u32 actualTime(void);			//return actual time
bool timeElapsed(u32 _time);	//return true if time elapsed

#endif
