

#include "stm32f30x_rcc.h"
#include "stm32f30x_gpio.h"

#include "timeBase.h"
#include "systemClock.h"
#include "uart.h"
#include "adc.h"
//#include "scope.h"
#include "generator.h"







void init_Discovery(void){

	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOE, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
}

void serialCommInit(void){
	COMM_InitTypeDef commInit;

	commInit.speed = COMM_NORMAL;	
	COMM_init(&commInit);
}

void initialize(void){
	
	GEN_InitTypeDef gGenSignal;
	GEN_SIGNAL sig;

	init_SystemClock();	  	//RCC config

	init_Discovery();	  	//leds & user button

	init_TimeBase();	  	//timebase & clock

	serialCommInit();		//uart2 init

	/*
	sig.amplitude = 0xFFF;
	sig.duty = 50;
	sig.frequency = 100;
	sig.GEN_signalType = GEN_SINE;
	sig.offset = 0;
	
	
	gGenSignal.p_GEN_signal = &sig;
	gGenSignal.p_GEN_buffer 
	*/
	
	/*
	gSCOPE.SCOPE_samplingFrequency = 10000;      
	gSCOPE.SCOPE_triggerEdge = SCOPE_RISING;
	gSCOPE.SCOPE_triggerMode = TRIG_AUTO;
	gSCOPE.SCOPE_triggerLevel = 2048;
	

	SCOPE_init(&gSCOPE);
	
	SCOPE_setPreTrigger(256);	
	*/


}
