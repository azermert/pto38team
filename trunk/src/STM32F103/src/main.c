//#include "stm32f10x.h"
//#include "stm32f10x_tim.h"
//#include "stm32f10x_adc.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
//#include "misc.h"

//#include "stdlib.h"
#include <string.h>
#include <stdio.h>

//#include "lcd.h"

//#include "HW\uart.h"
//#include "comm.h"
//#include "IO.h"
//#include "main_task.h"

//#include "globals.h"

#include "initialize.h"
#include "timeBase.h"
#include "comm.h"
#include "uart.h"
#include "adc.h"

#define FASTINTERVAL    500		//0.5ms
#define SLOWINTERVAL 2000000	//2000ms


/* Main variables ---------------------------------------------------------*/

typedef u32 time_t;

	time_t slowTick;
	time_t fastTick;
	u32 i = 0;
/* Function prototypes */

/* Functions */
int main(void) {

	

	initialize();
	slowTick = actualTime();
	fastTick = actualTime();

    /* program loop */		  //******************************************* main loop
	while (1) {


	if(timeElapsed(slowTick)){
		slowTick += SLOWINTERVAL;
		//COMM_send("test komunikace\n\r", 17 );
		//COMM_put_char('_');
		//COMM_put_char('X');
		//COMM_send("TEST KOMUNIKACE\n\r", 17 );

	}

	if(timeElapsed(fastTick)){
		fastTick += FASTINTERVAL;

	  	UART_tick();
	}

	if(gAdcMeasureDone){
	 	char msg[8] = {0,0,0,0,0,0,0,0};
		if(i < ADC_MEM_SIZE){
			sprintf(msg,"%d;\r\n",gAdcConvValues[i]);
			COMM_print(msg);
			i++;	
		}else{	
			gAdcMeasureDone = FALSE;
			COMM_print("DONE");
			i = 0;
		}
	}

	}  //end while
}  //end main()
