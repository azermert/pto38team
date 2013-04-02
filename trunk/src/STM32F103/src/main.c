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
//#include "timeBase.h"
//#include "systemClock.h"
//#include "singlePinPeriph.h"

//#include "HW\uart.h"
//#include "comm.h"
//#include "IO.h"
//#include "main_task.h"

//#include "globals.h"

#define FASTINTERVAL    500		//0.5ms
#define SLOWINTERVAL 200000		//200ms

#define OUTPUT_TIMEOUT 3000000	//3s

/* Main variables ---------------------------------------------------------*/
char 	message[16];
char echoMsg[128];

typedef struct TIME{
	u32 fast;
	u32 slow;
}TIME;

struct TIME time;

/* Function prototypes */
void init_main(void);
void init_ticks(void);
void init_Discovery(void);

//void ParseTime(u32 time);
void LCD_periodic_update(void);

void dummytick(void){
}

/* Functions */
int main(void) {
	u32 OutputEnableTimeout;

//	init_SystemClock();	   //RCC config
	init_Discovery();	   //leds & user button
//	init_TimeBase();	   //timebase & clock
//	init_LCD();			   //HD44780 based display
//	init_UART(&urt2,2);
//	init_IO();


    /* program loop */		  //******************************************* main loop
//	time.fast = actualTime() + FASTINTERVAL;
//	time.slow = actualTime() + SLOWINTERVAL;

	while (1) {

	//realtick:
/*	if(gValues.filter.output){
		//output enable, timeout restart
		OutputEnableTimeout = actualTime() + OUTPUT_TIMEOUT;
		GPIO_WriteBit(GPIOC, GPIO_Pin_9, (BitAction)1);
	}else if (timeElapsed(OutputEnableTimeout)){
		//output disable after timeout
		GPIO_WriteBit(GPIOC, GPIO_Pin_9, (BitAction)0);
	}


	if(timeElapsed(time.fast)){
		time.fast += FASTINTERVAL;
		//fasttick
		fast_Tick();

	}


	if(timeElapsed(time.slow)){
		time.slow += SLOWINTERVAL;
		//slowtick
		LCD_periodic_update();
		slow_Tick();

	}   */



	//COMM_DESC * PC_com = &urt2.parent;



	/*Ticks*/
	//PC_com->Tick(PC_com);
	//urt2.Tick(&urt2.parent);

	}  //end while
}  //end main()


void init_Discovery(void){

	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOA, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

/*void LCD_periodic_update(void){

		//LcdClear();
		LcdStrChar(lcd_line1, inst);
		LcdHome();
		sprintf(message,"%d",gValues.difference);
		LcdStrText(message);
		LcdStrText("       ");
		message[0]=0;
		message[1]=0;
		message[2]=0;
		message[3]=0;
		message[4]=0;
		message[5]=0;
		message[6]=0;
		message[7]=0;
		message[8]=0;
		message[9]=0;
		message[10]=0;
		message[11]=0;
		message[12]=0;
		message[13]=0;
		message[14]=0;
		message[15]=0;	

		LcdStrChar(lcd_line2, inst);
		sprintf(message,"%f",(float)gValues.adc_last_on  / (float)gValues.adc_last_off);
		LcdStrText(message);
		LcdStrText("       ");
		message[0]=0;
		message[1]=0;
		message[2]=0;
		message[3]=0;
		message[4]=0;
		message[5]=0;
		message[6]=0;
		message[7]=0;
		message[8]=0;
		message[9]=0;
		message[10]=0;
		message[11]=0;
		message[12]=0;
		message[13]=0;
		message[14]=0;
		message[15]=0;	
}  */


