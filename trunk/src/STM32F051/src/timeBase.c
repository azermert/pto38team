
#include "timeBase.h"
#include "NVIC_Basic.h"

#include "stm32f0xx.h"
#include "stm32f0xx_rcc.h"

/* Private variables*/
u32 CurrentTime;		//system time [microseconds]

#define SCLK_Freq (1000000/timeStep)
TIM_TimeBaseInitTypeDef Timer;

/* Function prototypes */
void SysTick_Handler(void);

/* Routines */
void init_TimeBase(void){
	NVIC_InitTypeDef invDef;
	RCC_ClocksTypeDef RCC_Clocks;
	RCC_GetClocksFreq(&RCC_Clocks);
	
	//zapnuti casovace TIM2 a nastaveni preruseni od nej (cca 4Khz)
	RCC_APB1PeriphClockCmd(RCC_APB1ENR_TIM2EN, ENABLE);
	
	Timer.TIM_Prescaler = 0;
	Timer.TIM_CounterMode = TIM_CounterMode_Down;
	Timer.TIM_Period = 4799;
	Timer.TIM_ClockDivision = TIM_CKD_DIV1;
	Timer.TIM_RepetitionCounter = 0;

	TIM_TimeBaseInit(TIM2, &Timer);	
	TIM_ITConfig(TIM2, TIM_DIER_UIE, ENABLE);
	TIM_ARRPreloadConfig(TIM2, ENABLE);
	TIM_Cmd(TIM2, ENABLE);
	
	invDef.NVIC_IRQChannel = TIM2_IRQn;
	invDef.NVIC_IRQChannelPriority = 1;
	invDef.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&invDef);	
	
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

void TIM2_IRQHandler (void){		//SysTick interrupt every 1000us		//irqn = 15
		CurrentTime += 100;//timeStep;
	
	TIM_ClearFlag(TIM2,TIM_FLAG_Update);
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);	
	NVIC_ClearPending(TIM2_IRQn);
	NVIC_ClearPendingIRQ(TIM2_IRQn);
	
}
