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

#include "hw_config.h"
#include "usb_lib.h"
#include "usb_desc.h"
#include "usb_pwr.h"

#define FASTINTERVAL	1000	//1ms
#define SLOWINTERVAL  100000	//100ms

/* Main variables ---------------------------------------------------------*/
GPIO_InitTypeDef GPIO_InitStructure;
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
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOE, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	

	//GPIO_WriteBit(GPIOE, GPIO_Pin_8, Bit_SET);
	
#if defined (USE_USB)	
	//USB init	
	//GPIOE->BSRR |= GPIO_Pin_8;
	Set_System();
	//GPIOE->BSRR |= GPIO_Pin_9;
  Set_USBClock();
	//GPIOE->BSRR |= GPIO_Pin_10;
  USB_Interrupts_Config();
	//GPIOE->BSRR |= GPIO_Pin_11;
  USB_Init();	
	//GPIOE->BSRR |= GPIO_Pin_12;
#endif

	

	//initialize();

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

