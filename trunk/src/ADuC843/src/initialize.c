

#include "ADuC843.h"
#include "typedef.h"

#include "timeBase.h"
#include "systemClock.h"
#include "uart.h"
#include "adc.h"
#include "scope.h"







void init_Discovery(void){
/*
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
	GPIO_Init(GPIOA, &GPIO_InitStructure);*/
}

void serialCommInit(void){
	COMM_InitTypeDef commInit;

	commInit.speed = COMM_NORMAL;	
	COMM_init(&commInit);
}

void initialize(void){
	PLLCON=0x0;
	CFG842|=0x01;	//nastaveni pristupu k vnejsi pameti
	EA=1;
	
	//init_SystemClock();	  	//RCC config

	init_Discovery();	  	//leds & user button

	init_TimeBase();	  	//timebase & clock

	serialCommInit();		//uart2 init

	
	
	gSCOPE.SCOPE_samplingFrequency = 10000;      
	gSCOPE.SCOPE_triggerEdge = SCOPE_RISING;
	gSCOPE.SCOPE_triggerMode = TRIG_NORMAL;
	gSCOPE.SCOPE_triggerLevel = 2048;
	

	SCOPE_init(&gSCOPE);
	
	SCOPE_setPreTrigger(256);	
	


}
