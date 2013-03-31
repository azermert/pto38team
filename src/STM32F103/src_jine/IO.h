#ifndef _IO_H_
#define _IO_H_

	#include "misc.h"
	#include "stm32f10x_gpio.h"


	#define ADC_Pin		GPIO_Pin_5
	#define ADC_Port	GPIOC
	#define ADC_CHANNEL		ADC_Channel_15
	#define ADC_CLOCKS 		RCC_APB2Periph_GPIOC|RCC_APB2Periph_ADC1

	/* Interrupt - analog watchdog */
	#define ADC			ADC1
	#define ADC_IRQn	ADC1_2_IRQn
	#define ADC_Handler		ADC_IRQHandler

	#define IR_Pin		GPIO_Pin_4
	#define IR_Port		GPIOC
	#define IR_PORT_CLK RCC_APB2Periph_GPIOC

typedef u8 OUTPUT_NO;

enum OUTPUT_NO{
	MAIN_OUT = 0,
	IR_LED,
	OUTPUT_CNT
};


void init_IO(void);
void setOutputState(OUTPUT_NO _pin, u8 _state);


#endif // _IO_H_
