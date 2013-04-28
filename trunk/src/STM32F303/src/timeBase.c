
#include "timeBase.h"

#include "stm32f30x.h"
#include "stm32f30x_rcc.h"

/* Private variables*/
u32 CurrentTime;		//system time [microseconds]

#define SCLK_Freq (1000000/timeStep)

/* Function prototypes */
void SysTick_Handler(void);

/* Routines */
void init_TimeBase(void){
	RCC_ClocksTypeDef RCC_Clocks;
	RCC_GetClocksFreq(&RCC_Clocks);
	
	//set timer	  SysTick_CLKSourceConfig( SysTick_CLKSource_HCLK );	
	SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk;
	SysTick->CTRL |= SysTick_CLKSource_HCLK;
	SysTick->LOAD = (RCC_Clocks.HCLK_Frequency/SCLK_Freq) - 1;
	SysTick->CTRL	|= SysTick_CTRL_ENABLE_Msk;  
}

void delay(u32 time){
	waitOnTime(CurrentTime + time);
}

void waitOnTime(u32 TrgtTime){
	while(TrgtTime > CurrentTime);
}

u32 actualTime(void){
	return CurrentTime;
}

bool timeElapsed(u32 _time){
 	if (CurrentTime > _time){
		return TRUE;
	}else{
		return FALSE;
	}
}



void SysTick_Handler (void){		//SysTick interrupt every 40us
	CurrentTime += timeStep;
}
