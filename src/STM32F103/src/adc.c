/**
  ******************************************************************************
  * @file    adc.c
  * @author  PTO Team
  * @version V1.0.0
  * @date    3/3/2013
  * @brief   Tato knihovna zajistuje funkci ADC (inicializace,sprava preruseni,
  *           kontrola preteceni bufferu....)
  *          Moznosti mereni jsou:
  *           - neustale kruhove mereni do bufferru
  *           - jednorazove mereni pomoci DMA
  *           - zmereni jednoho vzorku                
  *        
  ******************************************************************************  
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "stm32f10x_adc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_tim.h"
#include "adc.h"
#include "NVIC_Basic.h"
#include "timeBase.h"

#include "comm.h"
#include <string.h>
#include <stdio.h>

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/ 
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
bool lUnInitialized = TRUE;
bool lAdcOverflowed = FALSE;
ADC_STATE lAdcState = ADC_ERR;
PTO_ADC_InitTypeDef lAdc_desc;

uint16_t lAdcBufferPointer = 0;  // Ukazuje do bufferu na vzorek, ktery byl naposled precten
uint16_t lAdcBufferLastRead = 0;

uint16_t * lBuffPtr;
uint32_t lBuffSize;

volatile bool gAdcMeasureDone;	
uint16_t	gAdcConvValues[ADC_MEM_SIZE];

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/




/**
  * @brief  Vrati hodnotu namereneho vzorku (bud z pole, ktere postupne plni nebo odstartuje jendo mereni)
  * @param  None
  * @retval Namereni Vzorek
  */         
uint16_t ADC_meas_sample(void)
{
  //TODO
  return 0;
}           

/**
  * @brief  Odstartuje nepretrzite mereni a plneni bufferu do kruhu (pomoci casovace)
  * @param  None
  * @retval None
  */
void ADC_circle_meas_start(void)
{
  //TODO
}

/**
  * @brief  Zastavi nepretrzite mereni
  * @param  None
  * @retval None
  */
void ADC_circle_meas_stop(void)
{
  //TODO
}             

/**
  * @brief  Odstartuje jednorazove vzorkovani pomoci DMA do bufferu 
  * (pokud procesor nema DMA tak pomoci preruseni od casovace bude vzorkovat
  * stejne jako v circle mereni)
  * @param  None
  * @retval None
  */
void ADC_DMA_meas_start(void)
{
  //TODO
}           

/**
  * @brief  Vrati stav AD prevodniku
  * @param  None
  * @retval ADC_STATE - Stav ADC
  */
ADC_STATE ADC_get_state(void)
{
  return lAdcState;
}            

/**
  * @brief  Vrati ukazetel do bufferu na posledni namereny vzorek
  * @param  None
  * @retval uint16_t - index pole
  */
uint16_t ADC_get_pointer()
{
  return lAdcBufferPointer;
}               

/**
  * @brief  Nastavi index pole, ktery byl naposled precten/zpracovan
  * @param  uint16_t - index pole
  * @retval int8_t - 0 vse ok, -1 chyba
  */
int8_t ADC_set_last_read(uint16_t _index)
{
  int8_t result = -1;
  if (_index < lAdc_desc.ADC_memorySize){ /* Kontrola jestli index ukazuje do pole */ 
    lAdcBufferLastRead = _index;
    result = 0;
  }
  return result;
} 

/**
  * @brief  Zkontroluje pokud buffer pretekl
  * @param  None
  * @retval bool
  */
bool ADC_is_buffer_overflowed()
{
  return lAdcOverflowed;
}          

/**
  * @brief  Zpracovani preruseni
  * @param  None
  * @retval None
  */
void ADC_IRQHandler(void)
{
	static u32 time = 0;
	static u32 index = 0;

	if((!gAdcMeasureDone) && timeElapsed(time)){
		u16 tmpVal = ADC_GetConversionValue(ADC);
		lBuffPtr[index++] = tmpVal;
		
		if(index == lBuffSize){
			index = 0;
			gAdcMeasureDone = TRUE;
			time = actualTime() + 500000;
		}
	}

	ADC_ClearITPendingBit(ADC, ADC_IT_EOC);
	NVIC_ClearPending(ADC_IRQn);
}


u32 TIM_APB1_GetClockFrequency(void){
	RCC_ClocksTypeDef	RCC_Clocks;

 	RCC_GetClocksFreq(&RCC_Clocks);
		
	if(RCC_Clocks.HCLK_Frequency == RCC_Clocks.PCLK1_Frequency){
		return	RCC_Clocks.PCLK1_Frequency; 
	}else{
		return	2*(RCC_Clocks.PCLK1_Frequency);
	}
}

void Trigger_init(PTO_ADC_InitTypeDef * _desc){
	TIM_TimeBaseInitTypeDef 	TIM_TimeBaseInitStruct;
	TIM_OCInitTypeDef	TIM_OCInitStructure;
	s32 clk_div;
	u16 prescaler;
	u16 arr_reg;

	RCC_APB1PeriphClockCmd(TIMER_CLOCKS, ENABLE);
	
	TIM_DeInit(TRIGGER_TIMER);
	TIM_TimeBaseStructInit(&TIM_TimeBaseInitStruct);

	//sampling_frequency = frequency / (psc*arr)
	//clk_div = psc*arr
	clk_div = TIM_APB1_GetClockFrequency() / _desc->ADC_samplingFrequency;
	if(clk_div == 0){
		//error
		return;
	}else if(clk_div <= 0x0FFFF){
		prescaler = 0;
		arr_reg = clk_div - 1;
	}else{
		u32 err_val = 0x0FFFFFFFF;
		u32 err_min = 0x0FFFFFFFF;
		u32 err_min_ratio;
		u16 ratio = clk_div/0x0FFFF; 
		u16 div1;
		while(err_val != 0){
			ratio++;

			div1 = clk_div/ratio;
			err_val = clk_div - (div1*ratio);

			if(err_val < err_min){
				err_min = err_val;
				err_min_ratio = ratio;
			}
		
			if(ratio == 0x0FFFF){
				//nenasli jsme presnou kombinaci
				//dosadime nejlepsi zjistenou
				div1 = clk_div/err_min_ratio;
				ratio = err_min_ratio;
				err_val = 0;	//konec smycky
				break;
			}			
		}//while
		if(ratio > div1){
			prescaler = div1 -1;
			arr_reg = ratio - 1;		
		}else{
			prescaler = ratio -1;
			arr_reg = div1 - 1;
		}			
	}

	TIM_TimeBaseInitStruct.TIM_Prescaler = prescaler;
	TIM_TimeBaseInitStruct.TIM_Period = arr_reg;			
	TIM_TimeBaseInit(TRIGGER_TIMER, &TIM_TimeBaseInitStruct);

	//TIM_UpdateRequestConfig(TRIGGER_TIMER, TIM_UpdateSource_Regular);	//IRC_TIMER->CR1 = TIM_CR1_URS;
	TIM_ARRPreloadConfig(TRIGGER_TIMER, ENABLE);						//IRC_TIMER->CR1 |= TIM_CR1_ARPE;

	TIM_OCStructInit(&TIM_OCInitStructure);

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  	TIM_OCInitStructure.TIM_Pulse = 0x01;
  	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
  	TIM_OC4Init(TRIGGER_TIMER, &TIM_OCInitStructure);

	//TIM_SetCompare4(TRIGGER_TIMER, 1);					//CC4 for event generation
	
	TIM_Cmd(TRIGGER_TIMER, ENABLE);

}

void gpio_init(void){
	GPIO_InitTypeDef			GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(ADC_CLOCKS, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = ADC_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(ADC_Port, &GPIO_InitStructure);
}

/**
  * @brief  Inicializace AD prevodniku
  * @param  Descriptor obsahujici parametry pro nastaveni ADC
  * @retval None
  */
void ADC_init(PTO_ADC_InitTypeDef * _desc)
{
  //TODO
  /* 
  uint32_t ADC_samplingFrequency;        
  uint32_t * p_ADC_memory;                Pointer na pamet kam muze ADC zapisovat 
  uint32_t ADC_memorySize;                Velikost pameti pro ADC  
  tick_adc p_ADC_tick;
  */
  	ADC_InitTypeDef 		ADC_InitStruct;
	RCC_ClocksTypeDef		RCC_Clocks;
 	s32 					clocksPerSample;

	lBuffPtr = _desc->p_ADC_memory;
	lBuffSize = _desc->ADC_memorySize;

	RCC_APB2PeriphClockCmd(ADC_CLOCKS, ENABLE);

	RCC_GetClocksFreq(&RCC_Clocks);
	clocksPerSample = RCC_Clocks.ADCCLK_Frequency / _desc->ADC_samplingFrequency;
	
	ADC_DeInit(ADC);
	ADC_StructInit(&ADC_InitStruct);

	ADC_InitStruct.ADC_ExternalTrigConv = ADC_TRIGGER;
	ADC_InitStruct.ADC_NbrOfChannel = 1;
	ADC_Init(ADC, &ADC_InitStruct);

	ADC_ExternalTrigConvCmd(ADC, ENABLE);

	ADC_Cmd(ADC, ENABLE);	//wake up

	
	if(clocksPerSample > 239){
		ADC_RegularChannelConfig(ADC, ADC_CHANNEL, 1, ADC_SampleTime_239Cycles5);	
	}else if(clocksPerSample > 71){
		ADC_RegularChannelConfig(ADC, ADC_CHANNEL, 1, ADC_SampleTime_71Cycles5);
	}else if(clocksPerSample > 55){
		ADC_RegularChannelConfig(ADC, ADC_CHANNEL, 1, ADC_SampleTime_55Cycles5);
	}else if(clocksPerSample > 41){
		ADC_RegularChannelConfig(ADC, ADC_CHANNEL, 1, ADC_SampleTime_41Cycles5);
	}else if(clocksPerSample > 28){
		ADC_RegularChannelConfig(ADC, ADC_CHANNEL, 1, ADC_SampleTime_28Cycles5);
	}else if(clocksPerSample > 13){
		ADC_RegularChannelConfig(ADC, ADC_CHANNEL, 1, ADC_SampleTime_13Cycles5);
	}else if(clocksPerSample > 7){
		ADC_RegularChannelConfig(ADC, ADC_CHANNEL, 1, ADC_SampleTime_7Cycles5);
	}else{
		ADC_RegularChannelConfig(ADC, ADC_CHANNEL, 1, ADC_SampleTime_1Cycles5);
	}  
  
	ADC_ResetCalibration(ADC);
	while(ADC_GetResetCalibrationStatus(ADC));
	ADC_StartCalibration(ADC);
	while(ADC_GetCalibrationStatus(ADC));

	ADC_Cmd(ADC, ENABLE);	//start conversion

	ADC_ITConfig(ADC, ADC_IT_EOC, ENABLE);

	Trigger_init(_desc);
	gpio_init();

	NVIC_IntEnable(ADC_IRQn, 2);


} 
/************************ END OF FILE *****************************************/
