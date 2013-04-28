
#include "IO.h"

#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_adc.h"

#include "NVIC_Basic.h"

#include "globals.h"

void setOutputState(OUTPUT_NO _pin, u8 _state){

	switch (_pin){
		case IR_LED:
			GPIO_WriteBit(IR_Port, IR_Pin, (BitAction)_state);

		break;
	
	}

}

void ADC_Handler(void){
	u16 adc_last;

	if(ADC_GetFlagStatus(ADC, ADC_FLAG_EOC)){
	
		adc_last = ADC_GetConversionValue(ADC);
		gValues.adc_sum += adc_last;

	if (gValues.ir_State == IR_OFF){
		gValues.adc_last_off = adc_last;
	}else if (gValues.ir_State == IR_ON){
		gValues.adc_last_on = adc_last;
	}

	
		ADC_ClearITPendingBit(ADC, ADC_IT_AWD);
		NVIC_ClearPending(ADC_IRQn);
	}
}

void ADC_init(void){

	ADC_InitTypeDef 	ADC_InitStruct;
	GPIO_InitTypeDef	GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(ADC_CLOCKS, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = ADC_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(ADC_Port, &GPIO_InitStructure);

	ADC_DeInit(ADC);
	ADC_StructInit(&ADC_InitStruct);

	ADC_InitStruct.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
	ADC_InitStruct.ADC_NbrOfChannel = 1;
	ADC_Init(ADC, &ADC_InitStruct);

	ADC_Cmd(ADC, ENABLE);	//wake up

	ADC_RegularChannelConfig(ADC, ADC_CHANNEL, 1, ADC_SampleTime_239Cycles5);

	   
	ADC_ResetCalibration(ADC);
	while(ADC_GetResetCalibrationStatus(ADC));
	ADC_StartCalibration(ADC);
	while(ADC_GetCalibrationStatus(ADC));

	ADC_Cmd(ADC, ENABLE);	//start conversion

	ADC_ITConfig(ADC, ADC_IT_EOC, ENABLE);

	NVIC_IntEnable(ADC_IRQn, 2);
}

void GPIO_init(void){
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(IR_PORT_CLK, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = IR_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(IR_Port, &GPIO_InitStructure);


}


void init_IO(void){
	ADC_init();
	GPIO_init();
}
