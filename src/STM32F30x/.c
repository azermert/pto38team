
#include "systemClock.h"

#include "stm32f10x.h"
#include "stm32f10x_rcc.h"

#include "timeBase.h"

/* Private variables*/

/* Function prototypes */

/* Routines */
void init_SystemClock(void){

	RCC_DeInit();
	init_TimeBase();	//delay function init

	
   	RCC_HSEConfig(RCC_HSE_ON);
	if(RCC_WaitForHSEStartUp() != SUCCESS){
		while(1);	//user code to catch exception
	}

	RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_3); 	//SysClk = HSE*3
	RCC_PLLCmd(ENABLE);
	delay(500);
	if (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) != SET){
		while(1);	//user code to catch exception
	}

	//PLL successfully locked as 3xHSE	(24MHz)

   	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
	delay(500);
	if((RCC_GetSYSCLKSource() >> 2) != RCC_SYSCLKSource_PLLCLK){
		while(1);	//user code to catch exception
	}

	RCC_HCLKConfig(RCC_SYSCLK_Div1);   	//72MHz max
	RCC_PCLK1Config(RCC_HCLK_Div1);		//36MHz max
	RCC_PCLK2Config(RCC_HCLK_Div1);		//72MHz max

}
