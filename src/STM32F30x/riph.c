
#include "singlePinPeriph.h"

#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_adc.h"

#include "NVIC_Basic.h"


/* Functions */
void init_SinglePinPeriph(void){
	#ifdef IRC_INDEX
		init_Index();
	#endif

	#ifdef POSITION_MARK
		init_Mark();
	#endif

	#ifdef CURRENT_FB
		init_CurrentFB();
	#endif
}


#ifdef IRC_INDEX
	#ifndef IRC_InActiveState_HIGH
		#define	INDEX_LOW	INDEX_InActive
		#define	INDEX_HIGH	INDEX_Active			
	#else
		#define	INDEX_LOW	INDEX_Active
		#define	INDEX_HIGH	INDEX_InActive
	#endif
		
void init_Index(void){
	EXTI_InitTypeDef	EXTI_InitStruct;
	GPIO_InitTypeDef	GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(IRC_INDEX_GPIO_CLOCK, ENABLE);

	GPIO_InitStructure.GPIO_Pin = IRC_INDEX_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(IRC_INDEX_PORT, &GPIO_InitStructure);

	EXTI_StructInit(&EXTI_InitStruct);
	EXTI_InitStruct.EXTI_Line = IRC_INDEX_PIN;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	#ifndef IRC_InActiveState_HIGH
		EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;
	#endif
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;	
	EXTI_Init(&EXTI_InitStruct);

	GPIO_EXTILineConfig(IRC_EXTI_PortSource, IRC_EXTI_PinSource);

	indexIntEnable();
}

void indexIntEnable(void){
	NVIC_IntEnable(IRC_INDEX_IRQn, 1);
}

void indexIntDisable(void){
	NVIC_IntDisable(IRC_INDEX_IRQn);
}

void IRC_INDEX_Handler(void){

	EXTI_ClearITPendingBit(IRC_INDEX_PIN);
   	NVIC_ClearPending(IRC_INDEX_IRQn);
}
		
u8 getIndexState(void){
	if(GPIO_ReadInputDataBit(IRC_INDEX_PORT, IRC_INDEX_PIN) != Bit_RESET){
		return (u8)INDEX_HIGH;
	}else{
		return (u8)INDEX_LOW;
	}	
}
	
void waitToIndexInActive(void){
	while(getIndexState() != INDEX_InActive);
}

void waitToIndexActive(void){
	while(getIndexState() != INDEX_Active);
}

#endif //IRC_INDEX 	_________________________________________________

#ifdef POSITION_MARK
void init_Mark(void){
	GPIO_InitTypeDef	GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(POS_MARK_GPIO_CLOCK, ENABLE);

	GPIO_InitStructure.GPIO_Pin = POS_MARK_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(POS_MARK_PORT, &GPIO_InitStructure);
}

u8 getMarkState(void){
	return GPIO_ReadInputDataBit(POS_MARK_PORT, POS_MARK_PIN);	
}
	
void waitToMarkSet(void){
	while(getMarkState() == POS_MARK_Reset);
}

void waitToMarkReset(void){
	while(getMarkState() != POS_MARK_Reset);
}
#endif //POSITION_MARK ______________________________________________

#ifdef CURRENT_FB
/* Variables */
u8 OverCurrent_Flag = CURRENT_InLimit;
u16 Value_Offset;
u8	OverLoads = 0;

void init_CurrentFB(void){
	ADC_InitTypeDef 	ADC_InitStruct;
	GPIO_InitTypeDef	GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(CURRENT_IN_CLOCKS, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = CURRENT_IN_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(CURRENT_IN_PORT, &GPIO_InitStructure);

	ADC_DeInit(CURRENT_ADC);
	ADC_StructInit(&ADC_InitStruct);

	ADC_InitStruct.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
	ADC_InitStruct.ADC_NbrOfChannel = 1;
	ADC_Init(CURRENT_ADC, &ADC_InitStruct);

	ADC_Cmd(CURRENT_ADC, ENABLE);	//wake up

	ADC_RegularChannelConfig(CURRENT_ADC, CURRENT_ADC_CHANNEL, 1, ADC_SampleTime_239Cycles5);

	ADC_AnalogWatchdogSingleChannelConfig(CURRENT_ADC, CURRENT_ADC_CHANNEL);

	ADC_AnalogWatchdogCmd(CURRENT_ADC, ADC_AnalogWatchdog_SingleRegEnable);
	   
	ADC_ResetCalibration(CURRENT_ADC);
	while(ADC_GetResetCalibrationStatus(CURRENT_ADC));
	ADC_StartCalibration(CURRENT_ADC);
	while(ADC_GetCalibrationStatus(CURRENT_ADC));

	ADC_Cmd(CURRENT_ADC, ENABLE);	//start conversion

	{/* AWD treshold settings*/
		u16 LTR_SUM;
		while(!ADC_GetFlagStatus(CURRENT_ADC, ADC_FLAG_EOC));
		LTR_SUM = ADC_GetConversionValue(CURRENT_ADC);
		while(!ADC_GetFlagStatus(CURRENT_ADC, ADC_FLAG_EOC));
		LTR_SUM += ADC_GetConversionValue(CURRENT_ADC);
		while(!ADC_GetFlagStatus(CURRENT_ADC, ADC_FLAG_EOC));
		LTR_SUM += ADC_GetConversionValue(CURRENT_ADC);
		while(!ADC_GetFlagStatus(CURRENT_ADC, ADC_FLAG_EOC));
		LTR_SUM += ADC_GetConversionValue(CURRENT_ADC);

		Value_Offset = LTR_SUM / 4;	
	}	
	ADC_AnalogWatchdogThresholdsConfig(CURRENT_ADC, Value_Offset + CURRENT_HTR_ADD, 0);

	ADC_ITConfig(CURRENT_ADC, ADC_IT_AWD, ENABLE);

	NVIC_IntEnable(CURRENT_IRQn, 2);
}

float Current_mA(void){
	return (float)(ADC_GetConversionValue(CURRENT_ADC) - Value_Offset)*CURRENT_RESOLUTION_mA;
}

u8 Current_getFlag(void){
	return (u8)OverCurrent_Flag;
}

void Current_clearFlag(void){
	OverCurrent_Flag = CURRENT_InLimit;
}

void CURRENT_Handler(void){

	if(ADC_GetFlagStatus(CURRENT_ADC, ADC_FLAG_AWD)){
		OverCurrent_Flag = CURRENT_OverLoad;
		/* Over Current exception routine (call) */
		BRIDGE_SetIdle();
	
		ADC_ClearITPendingBit(CURRENT_ADC, ADC_IT_AWD);
		NVIC_ClearPending(CURRENT_IRQn);
	}
}

#endif //CURRENT_FB _________________________________________________
