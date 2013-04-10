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
#include "scope.h"
#include "abc.h"

#define FASTINTERVAL	1000	//1ms
#define SLOWINTERVAL  200000	//200ms

typedef enum uint32_t{		  //QM = question mark (?) ;  US = underscore	(_)
	REGISTER_ENUM(I,D,N,QM),	//IDN?
	REGISTER_ENUM(G,E,N,US),	//GEN_
	REGISTER_ENUM(G,P,I,O),
	REGISTER_ENUM(O,S,C,P),
	REGISTER_ENUM(L,O,G,US),
	REGISTER_ENUM(C,O,N,T),
	REGISTER_ENUM(M,E,A,S),
	REGISTER_ENUM(V,O,L,T),
	REGISTER_ENUM(S,E,T,V),
	REGISTER_ENUM(G,E,T,QM),
	REGISTER_ENUM(T,R,I,G),
	REGISTER_ENUM(L,E,V,L),
	REGISTER_ENUM(E,D,G,E),
	REGISTER_ENUM(F,R,E,Q),
	REGISTER_ENUM(D,E,P,T),
	REGISTER_ENUM(S,T,R,T),
	REGISTER_ENUM(S,T,O,P),
	REGISTER_ENUM(T,Y,P,E),
	REGISTER_ENUM(A,M,P,L),
	REGISTER_ENUM(O,F,F,S),
	REGISTER_ENUM(D,U,T,Y),
	REGISTER_ENUM(D,A,T,A),
	REGISTER_ENUM(C,H,A,N),
	REGISTER_ENUM(F,R,Q,QM),
	REGISTER_ENUM(P,L,S,QM),
	REGISTER_ENUM(V,A,L,QM)	
}UART_WORD;

//BUILD_WORD(A,B,C,D);

/* Main variables ---------------------------------------------------------*/

typedef u32 time_t;

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
	u32 i = 0;

	RQ_TRIGGER lTrig_request = RqSINGLE;
	bool singleShotEnable = FALSE;		//start single mereni osciloskopu

/* Function prototypes */
void measure_Tick(void);

void singleShotStart(void){
	singleShotEnable = TRUE;
}

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
	/*	SCOPE_set_trigger_mode(TRIG_SW_AUTO);
		SCOPE_set_sample_rate(20000);
		SCOPE_start_meas();
		delay(20000);
		SCOPE_set_trigger_mode(TRIG_SIGNAL); */
	}

	if(timeElapsed(fastTick)){
					u32 word;
					uint16_t status;
		fastTick += FASTINTERVAL;
	  	UART_tick();
		measure_Tick();
 
		/*switch (COMM_read_char()){
			case 3:		//nrm display start
				singleShotStart();
				break;
			case -1:
				break;
			case 0xAA:
					word = 0;
					status = COMM_read((uint8_t*)&word, 4);
					status += 0;
					COMM_send("parsed\n",7);
				break;
			default:
				break;
		}*/
		if( COMM_get_bytes_available() > 3){		
			status = COMM_read((uint8_t*)&word, 4);
			status += 0;
			COMM_send("parsed\n",7);
			switch(word){
				case IDNQM:
					COMM_send("Device\n",7);
					break;
				case GENUS:
					COMM_send("Generator\n",10);
					break;
				case GPIO:
					COMM_send("gpio\n",5);
					break;
				case OSCP:
					COMM_send("oscp\n",5);
					break;
				case CONT:
					COMM_send("cont\n",5);
					break;
				case MEAS:
					COMM_send("meas\n",5);
					break;
				case VOLT:
					COMM_send("volt\n",5);
					break;
				case LOGUS:
					COMM_send("Logic\n",5);
					break;
				default:
					break;	
			}		
		}
		 /*
		if(SCOPE_get_state() == SCOPE_DONE){
			u16 tmp;
			char val[4] = {0,0,0,0};
			COMM_send("new\n\r", 5);
			gSCOPE.p_SCOPE_buffer->readIndex = gSCOPE.p_SCOPE_buffer->indexStart;
			while(1){
				tmp = gSCOPE.p_SCOPE_buffer->memory[gSCOPE.p_SCOPE_buffer->readIndex];
				gSCOPE.p_SCOPE_buffer->readIndex++;
				if(gSCOPE.p_SCOPE_buffer->readIndex == gSCOPE.p_SCOPE_buffer->size){
					gSCOPE.p_SCOPE_buffer->readIndex = 0;
				}
				val[0] = 0;
				val[1] = 0;
				val[2] = 0;
				val[3] = 0;
				sprintf(val,"%d",tmp);
				COMM_send(val,4);
				COMM_put_char('\r');
				COMM_put_char('\n');
				if(gSCOPE.p_SCOPE_buffer->readIndex == gSCOPE.p_SCOPE_buffer->indexStart){
					//readDone
					break;
				}
			}
			SCOPE_stop_meas();
		}	 */

	}

	

	}  //end while
}  //end main()

void measure_Tick(void){


static STATE StmState = IDLE;
static time_t timeout;
static bool firstPass = TRUE;
u16 tmp;
//char val[4];


switch (StmState){

	case MEAS_AUTO:
		if(firstPass){
			SCOPE_set_trigger_mode(TRIG_SW_AUTO);
			SCOPE_start_meas();
			timeout = actualTime() + 1000000;  //1000ms timeDelay
			firstPass = FALSE;
		}
		if(timeElapsed(timeout)){
			if(lTrig_request == RqAUTO){
				SCOPE_SW_trigger();	//autoMode neceka na trigger		   
			}else{
				SCOPE_set_trigger_mode(TRIG_SIGNAL);
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
			if(gSCOPE.p_SCOPE_buffer->readIndex == gSCOPE.p_SCOPE_buffer->size){
				gSCOPE.p_SCOPE_buffer->readIndex = 0;
			}
	/*		val[0] = 0;
			val[1] = 0;
			val[2] = 0;
			val[3] = 0;
			sprintf(val,"%d",tmp);
			COMM_send(val,4);
			COMM_put_char('\r');
			COMM_put_char('\n');		 */

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
}
}














