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
#include "log_analyser.h"
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

void foo(uint16_t i) {
	i++;
}


/* Functions */
int main(void) {	
	LA_InitTypeDef td;
	LA_Buffer buf;
	uint16_t a,b;
	
	initialize();
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOE, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	

	//GPIO_WriteBit(GPIOE, GPIO_Pin_8, Bit_SET);
	
#if defined (USE_USB)	
	//USB init	
	Set_System();
  Set_USBClock();
  USB_Interrupts_Config();
  USB_Init();	
#endif

	buf.size = 1;
	buf.repeatMemory = &b;
	buf.sampleMemory = &a;

	td.LA_samplingFrequency = 100;
	td.p_LA_buffer = &buf;
	
	//LA_init(&td);

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
		//measure_Tick();
	}

	}  //end while
}  //end main()
