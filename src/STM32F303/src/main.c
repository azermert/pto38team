//#include "stm32f10x.h"
//#include "stm32f10x_tim.h"
//#include "stm32f10x_adc.h"
#include "stm32f30x_rcc.h"
#include "stm32f30x_gpio.h"
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

#include "stm32f30x.h"

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
/*
#define ID_STRING "STM32F100 based multipurpose device, K38FEL\r\n"

typedef enum uint32_t{		  	//QM = question mark (?) ;  US = underscore	(_)
	REGISTER_WID(I,D,N,QM),		//WID_IDNQM=('?' << 24)|('N' << 16)|('D' << 8)|('I'),
	REGISTER_WID(G,E,N,US),
	REGISTER_WID(G,P,I,O),
	REGISTER_WID(O,S,C,P),
	REGISTER_WID(L,O,G,US),
	REGISTER_WID(C,O,N,T),
	REGISTER_WID(M,E,A,S),
	REGISTER_WID(V,O,L,T),

	REGISTER_WID(S,E,T,V),
	REGISTER_WID(G,E,T,QM),

	REGISTER_WID(T,R,I,G),
	REGISTER_WID(L,E,V,L),
	REGISTER_WID(E,D,G,E),
	REGISTER_WID(F,R,E,Q),
	REGISTER_WID(D,E,P,T),
	REGISTER_WID(S,T,R,T),
	REGISTER_WID(S,T,O,P),

	REGISTER_WID(T,Y,P,E),
	REGISTER_WID(A,M,P,L),
	REGISTER_WID(O,F,F,S),
	REGISTER_WID(D,U,T,Y),
	REGISTER_WID(D,A,T,A),

	REGISTER_WID(C,H,A,N),

	REGISTER_WID(F,R,Q,QM),
	REGISTER_WID(P,L,S,QM),

	REGISTER_WID(V,A,L,QM),

	REGISTER_WID(N,O,R,M),
	REGISTER_WID(A,U,T,O),
	REGISTER_WID(S,I,N,G),

	REGISTER_WID(R,I,S,E),
	REGISTER_WID(F,A,L,L),

	REGISTER_WID(1,K,US,US),
	REGISTER_WID(1,0,K,US),
	REGISTER_WID(1,0,0,K),
	REGISTER_WID(1,M,US,US),

	REGISTER_WID(0,8,B,US),
	REGISTER_WID(1,6,B,US),

	REGISTER_WID(S,I,N,E),
	REGISTER_WID(S,Q,R,E),
	REGISTER_WID(T,R,I,A),
	REGISTER_WID(A,R,B,T),

	REGISTER_WID(A,N,D,US),
	REGISTER_WID(O,R,US,US)
}WORD_ID;

//BUILD_WORD(A,B,C,D);
*/
/* Main variables ---------------------------------------------------------*/
typedef void (*readTick)(void);

readTick messageParser;

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
	//bool singleShotEnable = FALSE;		//start single mereni osciloskopu

/* Function prototypes */
void measure_Tick(void);
//void messageReadTick(void);


/* Functions */
int main(void) {

	

	initialize();

//	messageParser = &messageReadTick;	//default

	slowTick = actualTime();
	fastTick = actualTime();

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
		measure_Tick();
	}
	



	}  //end while
}  //end main()


/*
void discardMessage(void){
	static bool msgEnd = FALSE;
	char ch = COMM_view_char();

	switch (ch){
		case '\r':
		case '\n':
			COMM_read_char();
			msgEnd = TRUE;
			break;
		case -1:
			break;
		default:
			if(msgEnd){
				messageParser = &messageReadTick;
				msgEnd = FALSE;
			}else{
				COMM_read_char();
			}
	}
}
*/
/*
void oscpMsgParser(void){
	WORD_ID cmd;
	if( COMM_get_bytes_available() > 4){
		if(COMM_read_char() != ':'){
			messageParser = &discardMessage;
			return;
		}		
		COMM_read((uint8_t*)&cmd, 4);

		switch(cmd){
			case WID_TRIG:				
				break;
			case WID_LEVL:
				break;
			case WID_EDGE:
				break;
			case WID_FREQ:
				break;
			case WID_DEPT:
				break;
			case WID_STRT:
				break;
			case WID_STOP:
				break;

			default:
				break;	
		}		
	}
}
*/

/*
void messageReadTick(void){
	WORD_ID word;
	if( COMM_get_bytes_available() > 3){		
		COMM_read((uint8_t*)&word, 4);

		switch(word){
			case WID_IDNQM:
				COMM_print(ID_STRING);
				break;
			case WID_GENUS:
				COMM_print("Generator unimplemented\n");
				break;
			case WID_GPIO:
				COMM_print("Gpio unimplemented\n");
				break;
			case WID_OSCP:
					messageParser = &oscpMsgParser;
				break;
			case WID_CONT:
				COMM_send("cont\n",5);
				break;
			case WID_MEAS:
				COMM_send("meas\n",5);
				break;
			case WID_VOLT:
				COMM_send("volt\n",5);
				break;
			case WID_LOGUS:
				COMM_send("Logic\n",5);
				break;
			default:
				break;	
		}		
	}
}*/

