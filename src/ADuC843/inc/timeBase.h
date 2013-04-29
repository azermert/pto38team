#ifndef _TIMEBASE_H
#define _TIMEBASE_H

//#include "misc.h"

#define timeStep 500	//microseconds

/* Accesible function prototypes */
void init_TimeBase(void);

void delay(uint32_t time);			//time delay, wait specified amount of time
void waitOnTime(uint32_t trgtTime);	//wait until target time is reached
uint32_t actualTime(void);			//return actual time
bool timeElapsed(uint32_t _time);	//return true if time elapsed

#endif
