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
#include "stm32f30x.h"
#include "stm32f30x_adc.h"
#include "stm32f30x_gpio.h"
#include "stm32f30x_rcc.h"
#include "stm32f30x_tim.h"
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
//bool lUnInitialized = TRUE;
bool lAdcOverflowed = FALSE;
uint16_t lAdcBufferPointer = 0;  
uint16_t lAdcBufferLastRead = 0;	// Ukazuje do bufferu na vzorek, ktery byl naposled precten					


tick_adc lScopeTick = NULL;
ADC_STATE lAdcState = ADC_IDLE;
	
uint16_t	lAdcConvValues[ADC_MEM_SIZE];
uint16_t 	lBuffIndex;
uint16_t 	lBuffSize;

ADC_InitTypeDef 		ADC_InitStructure;
ADC_CommonInitTypeDef ADC_CommonInitStructure;
RCC_ClocksTypeDef		RCC_Clocks;
NVIC_InitTypeDef 		NVIC_InitStructure;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/




/**
  * @brief  Vrati hodnotu namereneho vzorku (bud z pole, ktere postupne plni nebo odstartuje jendo mereni)
  * @param  None
  * @retval Namereni Vzorek
  */         
uint16_t ADC_meas_sample(void)
{
  u16 i = 0;
  u32 sum = 0;
	
  while(i < lBuffSize){
  	sum += lAdcConvValues[i];
	i++;
  }
  return sum/lBuffSize;		//vrati prumer z celeho pole
}           

/**
  * @brief  Odstartuje nepretrzite mereni a plneni bufferu do kruhu (pomoci casovace)
  * @param  None
  * @retval None
  */
void ADC_circle_meas_start(void)
{
	
	ADC_StartConversion(ADC);
	lAdcState = ADC_RUN_INF;
}

/**
  * @brief  Zastavi nepretrzite mereni
  * @param  None
  * @retval None
  */
void ADC_circle_meas_stop(void)
{
  ADC_StopConversion(ADC);
	lAdcState = ADC_IDLE;
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
  if (_index < lBuffSize){ /* Kontrola jestli index ukazuje do pole */ 
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
void ADC1_2_IRQHandler(void)
{
	static uint16_t tmp;
	//u8	ISx;
	
	switch(lAdcState){
		case ADC_RUN_INF:

			tmp = ADC->DR;//ADC_GetConversionValue(ADC);	  //snaha o rychlost .. =D

			if(lBuffIndex == lBuffSize){
				lBuffIndex = 0;
			}

			lAdcConvValues[lBuffIndex++] = tmp;

		//	if(lScopeTick != NULL){
				lScopeTick(tmp);
		//	}
			break;

		case ADC_DMA_RUN:
		case ADC_DMA_DONE:
	   	case ADC_IDLE:
	   	case ADC_ERR:
	   	default:
	   		break;
	}//switch

	
	//ADC_ClearITPendingBit(ADC, ADC_IT_EOC);
	  	/* Clear the selected ADC interrupt pending bits */
	ADC_ClearITPendingBit(ADC, ADC_IT_EOC);
	//NVIC_ClearPending(ADC_IRQn);
	//	ISx = ADC_IRQn/32;	//=0	  ADC_Irqn = 18
		NVIC->ICPR[0] |= (0x01 << (ADC_IRQn));
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
	
	//RCC_APB2PeriphClockCmd(ADC_CLOCKS, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = ADC_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(ADC_Port, &GPIO_InitStructure);
}

/**
  * @brief  Inicializace AD prevodniku
  * @param  Descriptor obsahujici parametry pro nastaveni ADC
  * @retval None
  */
void ADC_init(PTO_ADC_InitTypeDef * _desc)
{
 	s32 					clocksPerSample;
	uint16_t i;

	lBuffIndex = 0;
	lBuffSize = ADC_MEM_SIZE;
	lScopeTick = _desc->p_ADC_tick;

	gpio_init();
	
	//RCC_AHBPeriphClockCmd(ADC_CLOCKS, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_ADC12, ENABLE);
	//RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);

	RCC_GetClocksFreq(&RCC_Clocks);
	clocksPerSample = RCC_Clocks.ADC12CLK_Frequency / _desc->ADC_samplingFrequency;
	
	ADC_DeInit(ADC);
	ADC_StructInit(&ADC_InitStructure);

	/* Enable the ADC gloabal Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = ADC_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	NVIC_ClearPending(ADC_IRQn);
	
	/*kalibrace ADC*/
	/* Calibration procedure */ 
  ADC_VoltageRegulatorCmd(ADC1, ENABLE);
  
  /* Insert delay equal to 10 �s */
  //Delay(10);
	for(i = 0; i < 3000; i++)
		__nop();
		

  
  ADC_SelectCalibrationMode(ADC1, ADC_CalibrationMode_Single);
  ADC_StartCalibration(ADC1);
  
  //calibration_value = ADC_GetCalibrationValue(ADC1);
     
	
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;                                                                    
  ADC_CommonInitStructure.ADC_Clock = ADC_Clock_SynClkModeDiv1;//ADC_Clock_AsynClkMode;                    
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;             
  ADC_CommonInitStructure.ADC_DMAMode = ADC_DMAMode_OneShot;                  
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = 0;          
  ADC_CommonInit(ADC, &ADC_CommonInitStructure);

  ADC_InitStructure.ADC_ExternalTrigConvEvent = ADC_TRIGGER;
	ADC_InitStructure.ADC_ContinuousConvMode = ADC_ContinuousConvMode_Enable;
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;         
  ADC_InitStructure.ADC_ExternalTrigEventEdge = ADC_ExternalTrigEventEdge_RisingEdge;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_OverrunMode = ADC_OverrunMode_Disable;   
  ADC_InitStructure.ADC_AutoInjMode = ADC_AutoInjec_Disable;  
  ADC_InitStructure.ADC_NbrOfRegChannel = 1;
  ADC_Init(ADC, &ADC_InitStructure);
	
	ADC_ExternalTriggerConfig(ADC, ADC_TRIGGER ,ADC_ExternalTrigEventEdge_RisingEdge);

	ADC_Cmd(ADC, ENABLE);	//wake up

	
	if(clocksPerSample > 601){
		ADC_RegularChannelConfig(ADC, ADC_CHANNEL, 1, ADC_SampleTime_601Cycles5);	
	}else if(clocksPerSample > 181){
		ADC_RegularChannelConfig(ADC, ADC_CHANNEL, 1, ADC_SampleTime_181Cycles5);
	}else if(clocksPerSample > 61){
		ADC_RegularChannelConfig(ADC, ADC_CHANNEL, 1, ADC_SampleTime_61Cycles5);
	}else if(clocksPerSample > 19){
		ADC_RegularChannelConfig(ADC, ADC_CHANNEL, 1, ADC_SampleTime_19Cycles5);
	}else if(clocksPerSample > 7){
		ADC_RegularChannelConfig(ADC, ADC_CHANNEL, 1, ADC_SampleTime_7Cycles5);
	}else if(clocksPerSample > 4){
		ADC_RegularChannelConfig(ADC, ADC_CHANNEL, 1, ADC_SampleTime_4Cycles5);
	}else if(clocksPerSample > 2){
		ADC_RegularChannelConfig(ADC, ADC_CHANNEL, 1, ADC_SampleTime_2Cycles5);
	}else{
		ADC_RegularChannelConfig(ADC, ADC_CHANNEL, 1, ADC_SampleTime_1Cycles5);
	}  

	ADC_Cmd(ADC, ENABLE);	//start conversion

	ADC_ITConfig(ADC, ADC_IT_EOC, ENABLE);

	Trigger_init(_desc);
	NVIC_IntEnable(ADC_IRQn, 4);
	

} 
/************************ END OF FILE *****************************************/
