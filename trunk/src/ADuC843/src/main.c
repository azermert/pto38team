//#include "stm32f10x.h"
//#include "stm32f10x_tim.h"
//#include "stm32f10x_adc.h"

#pragma large

#include "ADuC843.h"
#include "typedef.h"
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
#include "scope.h"
//#include "abc.h"
#include "state_automat.h"

#define FASTINTERVAL	1000	//1ms
#define SLOWINTERVAL  100000	//100ms

sbit TL=P3^2;
sbit LED=P3^4;
uint32_t ik,jk;
/* Main variables ---------------------------------------------------------*/
typedef void (*readTick)(void);

readTick messageParser;

typedef uint32_t time_t;

typedef enum {
	MEAS_NORM,
	MEAS_AUTO,
	SEND_DATA,
	IDLE
}STATE;

typedef enum
{
	RqNORMAL,
	RqAUTO,
	RqSINGLE
}RQ_TRIGGER;

	time_t slowTick;
	time_t fastTick;
	uint32_t i = 0;
	RQ_TRIGGER lTrig_request = RqSINGLE;
	
/* Function prototypes */
//void measure_Tick(void);
//void messageReadTick(void);

/*void singleShotStart(void){
	singleShotEnable = TRUE;
}*/

/* Functions */
int main(void) {
//	CFG842|=0x80;
	//SP=0x01;
	
	
	
	initialize();
	
//	messageParser = &messageReadTick;	//default

	slowTick = actualTime();
	fastTick = actualTime();
	COMM_print("Hello");	

    /* program loop */		  //******************************************* main loop
	
	while (1) {
		
	if(timeElapsed(slowTick)){
		slowTick += SLOWINTERVAL;
		//messageParser();
		STATE_tick_slow();
	}

	if(timeElapsed(fastTick)){
		fastTick += FASTINTERVAL;

		STATE_tick_fast();
	  UART_tick();
		
	}
	



	}  //end while
}  //end main()

/*void measure_Tick(void){

static STATE StmState;
static time_t timeout;
static bool firstPass = TRUE;
uint16_t tmp;

switch (StmState){

	case MEAS_AUTO:
		if(firstPass){
			SCOPE_set_trigger_mode(TRIG_AUTO);
			SCOPE_start_meas();
			timeout = actualTime() + 1000000;  //1000ms timeDelay
			firstPass = FALSE;
		}
		if(timeElapsed(timeout)){
			if(lTrig_request == RqAUTO){
				SCOPE_SW_trigger();	//autoMode neceka na trigger		   
			}else{
				SCOPE_set_trigger_mode(TRIG_NORMAL);
			}
			StmState = MEAS_NORM;
		}
	   	break;

	case MEAS_NORM:										 //ceka na domereni dat po triggeru
		if(SCOPE_get_state() == SCOPE_DONE){
			StmState = SEND_DATA;	  			//measure finished
			SCOPE_stop_meas();
			firstPass = TRUE;
		}
		break;

   	case SEND_DATA:

			if(COMM_get_out_buff_state() == BUFF_ALMOST_FULL){
				break;
			}

			if(firstPass){
			//	COMM_send("new\n\r", 5);
				COMM_put_char(0x03);	   //nrmdisplay start
				gSCOPE.p_SCOPE_buffer->readIndex = gSCOPE.p_SCOPE_buffer->indexStart;
				firstPass = FALSE;
			}
			
			tmp = gSCOPE.p_SCOPE_buffer->memory[gSCOPE.p_SCOPE_buffer->readIndex];
			gSCOPE.p_SCOPE_buffer->readIndex++;
			if(gSCOPE.p_SCOPE_buffer->readIndex == gSCOPE.p_SCOPE_buffer->size_buff){
				gSCOPE.p_SCOPE_buffer->readIndex = 0;
			}

			tmp = (tmp >> 4);	//12 bit -> 8 bit
			tmp = (tmp & 0x0FF);
			COMM_put_char((uint8_t)tmp);			

			if(gSCOPE.p_SCOPE_buffer->readIndex == gSCOPE.p_SCOPE_buffer->indexStart){
				//readDone
				StmState = IDLE;
			}
		break;

	case IDLE:
	default:
		if(lTrig_request != RqSINGLE){
			StmState = MEAS_AUTO;
			firstPass = TRUE;
		}
		else if(singleShotEnable)
		{
			singleShotEnable = FALSE;
			StmState = MEAS_AUTO;
			firstPass = TRUE;	
		}
		break;
}//switch
}//measure tick*/

