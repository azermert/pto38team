


#include "ADuC843.h"
#include "typedef.h"
#include "timeBase.h"
/* Private variables*/
uint32_t CurrentTime;		//system time [microseconds]

#define SCLK_Freq (1000000/timeStep)

/* Function prototypes */
void SysTick_Handler(void);

/* Routines */
void init_TimeBase(void){
	ET0=1; //povoleni preruseni Timer 0
	TH0=0;
	TL0=0;
	TMOD=0x00;
	TCON=0x11;
	
/*	RCC_ClocksTypeDef RCC_Clocks;
	RCC_GetClocksFreq(&RCC_Clocks);
	
	//set timer	  SysTick_CLKSourceConfig( SysTick_CLKSource_HCLK );
	SysTick->CTRL |= SysTick_CTRL_CLKSOURCE|SysTick_CTRL_TICKINT;	//Processor clk (HCLK), IRQ enable
	SysTick->LOAD = (RCC_Clocks.HCLK_Frequency/SCLK_Freq) - 1;
	SysTick->CTRL |= SysTick_CTRL_ENABLE;*/
}

void delay(uint32_t time){
	waitOnTime(CurrentTime + time);
}

void waitOnTime(uint32_t TrgtTime){
	while(TrgtTime > CurrentTime);
}

uint32_t actualTime(void){
	return CurrentTime;
}

bool timeElapsed(uint32_t _time){
 	if (CurrentTime > _time){
		return TRUE;
	}else{
		return FALSE;
	}
}



void SysTick_Handler (void) interrupt 1{		//SysTick interrupt every 40us
	EA=0;
	CurrentTime += timeStep;
	TH0=0;
	TL0=0;
	EA=1;
}
