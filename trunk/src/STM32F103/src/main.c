//#include "stm32f10x.h"
//#include "stm32f10x_tim.h"
//#include "stm32f10x_adc.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
//#include "misc.h"

//#include <string.h>
//#include <stdio.h>

#include "stm32f10x.h"

#include "initialize.h"
#include "timeBase.h"
#include "comm.h"
#include "uart.h"
#include "adc.h"
#include "dac.h"
#include "scope.h"
#include "state_automat.h"

#define FASTINTERVAL	1000	//1ms
#define SLOWINTERVAL  100000	//100ms

/* Main variables ---------------------------------------------------------*/
	time_t slowTick;
	time_t fastTick;

/* Function prototypes */

/* Functions */
int main(void) {

	initialize();


	slowTick = actualTime();
	fastTick = actualTime();

    /* program loop */		  //******************************************* main loop
	while (1) {

	if(timeElapsed(slowTick)){
		//u16 sample;
		
		slowTick += SLOWINTERVAL;
		//messageParser();
		STATE_tick_slow();

		//DAC_set_sample(sample);


	}

	if(timeElapsed(fastTick)){
		fastTick += FASTINTERVAL;

		STATE_tick_fast();
	  UART_tick();
	}
	
	}  //end while
}  //end main()


