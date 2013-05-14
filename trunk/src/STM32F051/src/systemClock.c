
#include "systemClock.h"

#include "stm32f0xx.h"
#include "typedefs.h"
#include "stm32f0xx_rcc.h"

#include "timeBase.h"

/* Private variables*/

/* Function prototypes */

/* Routines */
void init_SystemClock(void){

	RCC_DeInit();
	init_TimeBase();	//delay function init

	
//    	RCC_HSEConfig(RCC_HSE_ON);
// 	if(RCC_WaitForHSEStartUp() != SUCCESS){
// 		while(1);	//user code to catch exception
// 	}
// delicka hodin v pomeru RCC_PllMul v stm32_rcc pAK KONFIG V PLLConfig
	
	//************************************************
	//RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_3); 	//SysClk = HSE*3
	//????
	
// 	RCC->CR|=0x1;//zapnuti HSI
// 	while((RCC->CR&0x2)==0){}	//je HSI pripraveno?
// 	RCC->CR|=0x1000000;				//zapnuti PLL
// 	while((RCC->CR&0x2000000)==0){}//cekani az bude PLL pripraveno
	
	RCC_PLLConfig(RCC_PLLSource_HSI_Div2, RCC_PLLMul_3); 	//SysClk = HSI*3
	
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
	
// 	RCC_HCLKConfig(RCC_SYSCLK_Div1);   	//72MHz max
// 	RCC_PCLK1Config(RCC_HCLK_Div1);		//36MHz max
// 	RCC_PCLK2Config(RCC_HCLK_Div1);		//72MHz max
//** upravovano...
	RCC_HCLKConfig(RCC_SYSCLK_Div1);   	//72MHz max
	//**************************************
	RCC_PCLKConfig(RCC_HCLK_Div1);		//36MHz max
	//RCC_PCLKConfig(RCC_HCLK_Div1);		//72MHz max

}
