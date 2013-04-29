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
#include "stm32f0xx.h"
#include "stm32f0xx_conf.h"
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
bool ADC_unInitialized = TRUE;
bool lAdcOverflowed = FALSE;
ADC_STATE adcState = ADC_ERR;
PTO_ADC_InitTypeDef ADC_desc;

uint16_t lAdcBufferPointer = 0;  
uint16_t lAdcBufferLastRead = 0;	// Ukazuje do bufferu na vzorek, ktery byl naposled precten					


tick_adc lScopeTick = NULL;
ADC_STATE lAdcState = ADC_IDLE;
	
uint16_t	lAdcConvValues[ADC_MEM_SIZE];
uint16_t 	lBuffIndex;
uint16_t 	lBuffSize;

//__IO uint16_t  ADC1ConvertedValue = 0, ADC1ConvertedVoltage = 0;
/* Private function prototypes -----------------------------------------------*/
//void ADC1_Config(void);
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Inicializace AD prevodniku
  * @param  Descriptor obsahujici parametry pro nastaveni ADC
  * @retval None
  */
// void ADC_init(PTO_ADC_InitTypeDef * p_ADC_desc)
// {
//    ADC1_Config();
// } 

/**
  * @brief  Vrati hodnotu namereneho vzorku (bud z pole, ktere postupne plni nebo odstartuje jendo mereni)
  * @param  None
  * @retval Namereni Vzorek
  */         
uint16_t ADC_meas_sample()
{
  u16 i = 0;
  u32 sum = 0;
  while(i < lBuffSize){
  	sum += lAdcConvValues[i];
	i++;
  }
  return sum/lBuffSize;		//vrati prumer z celeho pol
}           

/**
  * @brief  Odstartuje nepretrzite mereni a plneni bufferu do kruhu (pomoci casovace)
  * @param  None
  * @retval None
  */
void ADC_circle_meas_start()
{
	lAdcState = ADC_RUN_INF;
  //ADC_StartOfConversion(ADC1);
}

/**
  * @brief  Zastavi nepretrzite mereni
  * @param  None
  * @retval None
  */
void ADC_circle_meas_stop()
{
	lAdcState = ADC_IDLE;
  //ADC_StopOfConversion(ADC1);
}             

/**
  * @brief  Vrati stav AD prevodniku
  * @param  None
  * @retval ADC_STATE - Stav ADC
  */
ADC_STATE ADC_get_state()
{
	//ADC1ConvertedValue = ADC_GetConversionValue(ADC1);
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
  * @brief  Zkontrolije pokud buffer pretekl
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
void ADC1_COMP_IRQHandler(void)
{
  static uint16_t tmp;
	//u8	ISx;

	switch(lAdcState){
		case ADC_RUN_INF:

		//	tmp = ADC1->DR;
			tmp = ADC_GetConversionValue(ADC1);	  //snaha o rychlost .. =D

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
	  ADC1->ISR = (uint32_t)ADC_IT_EOC;
	//NVIC_ClearPending(ADC_IRQn);
	//	ISx = ADC_IRQn/32;	//=0	  ADC_Irqn = 18
		NVIC->ICPR[0] |= (0x01 << (ADC_IRQn));
}

u32 TIM_APB1_GetClockFrequency(void){
	RCC_ClocksTypeDef	RCC_Clocks;

 	RCC_GetClocksFreq(&RCC_Clocks);
		
	if(RCC_Clocks.HCLK_Frequency == RCC_Clocks.PCLK_Frequency){
		return	RCC_Clocks.PCLK_Frequency; 
	}else{
		return	2*(RCC_Clocks.PCLK_Frequency);
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
	TIM_SelectOutputTrigger(TIM3, TIM_TRGOSource_Update);
}


void gpio_init(void){
	GPIO_InitTypeDef			GPIO_InitStructure;

	//RCC_APB2PeriphClockCmd(ADC_CLOCKS, ENABLE);
	/* GPIOC Periph clock enable */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
  /* ADC1 Periph clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = ADC_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
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

  ADC_InitTypeDef 		ADC_InitStruct;
	RCC_ClocksTypeDef		RCC_Clocks;
 	s32 					clocksPerSample;

	lBuffIndex = 0;
	lBuffSize = ADC_MEM_SIZE;
	lScopeTick = _desc->p_ADC_tick;

// 	RCC_APB2PeriphClockCmd(ADC_CLOCKS, ENABLE);
	/* GPIOC Periph clock enable */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
  /* ADC1 Periph clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

	RCC_GetClocksFreq(&RCC_Clocks);
	clocksPerSample = RCC_Clocks.ADCCLK_Frequency / _desc->ADC_samplingFrequency;
	
	ADC_DeInit(ADC1);
	ADC_StructInit(&ADC_InitStruct);

	ADC_InitStruct.ADC_ExternalTrigConv = ADC_TRIGGER;
	ADC_InitStruct.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_Rising;
//	ADC_InitStruct.ADC_NbrOfChannel = 1;
	ADC_Init(ADC1, &ADC_InitStruct);


	ADC_Cmd(ADC1, ENABLE);	//wake up

	
	if(clocksPerSample > 239){
		ADC_ChannelConfig(ADC1, ADC_CHANNEL, ADC_SampleTime_239_5Cycles);	
	}else if(clocksPerSample > 71){
		ADC_ChannelConfig(ADC1, ADC_CHANNEL, ADC_SampleTime_71_5Cycles);
	}else if(clocksPerSample > 55){
		ADC_ChannelConfig(ADC1, ADC_CHANNEL, ADC_SampleTime_55_5Cycles);
	}else if(clocksPerSample > 41){
		ADC_ChannelConfig(ADC1, ADC_CHANNEL, ADC_SampleTime_41_5Cycles);
	}else if(clocksPerSample > 28){
		ADC_ChannelConfig(ADC1, ADC_CHANNEL, ADC_SampleTime_28_5Cycles);
	}else if(clocksPerSample > 13){
		ADC_ChannelConfig(ADC1, ADC_CHANNEL, ADC_SampleTime_13_5Cycles );
	}else if(clocksPerSample > 7){
		ADC_ChannelConfig(ADC1, ADC_CHANNEL, ADC_SampleTime_7_5Cycles );
	}else{
		ADC_ChannelConfig(ADC1, ADC_CHANNEL, ADC_SampleTime_1_5Cycles);
	}  
  
	
// 	ADC_ResetCalibration(ADC1);
// 	while(ADC_GetResetCalibrationStatus(ADC1));
// 	ADC_StartCalibration(ADC1);
// 	while(ADC_GetCalibrationStatus(ADC1));
// 	ADC_Cmd(ADC1, ENABLE);	//start conversion
	
	/* ADC Calibration */
  ADC_GetCalibrationFactor(ADC1);
  /* Enable the auto delay feature */    
  ADC_WaitModeCmd(ADC1, ENABLE); 
  /* Enable the Auto power off mode */
  ADC_AutoPowerOffCmd(ADC1, ENABLE); 
  /* Enable ADCperipheral[PerIdx] */
  ADC_Cmd(ADC1, ENABLE);     

	ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);
/* ADC1 regular Software Start Conv */ 
  ADC_StartOfConversion(ADC1);
	
	Trigger_init(_desc);
	gpio_init();

	NVIC_IntEnable(ADC_IRQn, 4);

} 


// void ADC1_Config(void)
// {
//   ADC_InitTypeDef          ADC_InitStructure;
//   GPIO_InitTypeDef         GPIO_InitStructure;
//   TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
//   TIM_OCInitTypeDef        TIM_OCInitStructure; 
//   
//   /* GPIOC Periph clock enable */
//   RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
//   
//   /* ADC1 Periph clock enable */
//   RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
//   
//   /* TIM3 Periph clock enable */
//   RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
//   
//   /* Configure ADC Channel11 as analog input */
//   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 ;
//   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
//   GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
//   GPIO_Init(GPIOC, &GPIO_InitStructure);
//   
//   /* TIM3 Configuration *******************************************************/
//   TIM_DeInit(TIM3);
//   
//   TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
//   
//   TIM_OCStructInit(&TIM_OCInitStructure);
//     
//   /* Time base configuration */
//   TIM_TimeBaseStructure.TIM_Period = 0xFF;
//   TIM_TimeBaseStructure.TIM_Prescaler = 0x0;
//   TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;
//   TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
//   TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
//   
//   /* TIM3 TRGO selection */
//   TIM_SelectOutputTrigger(TIM3, TIM_TRGOSource_Update);
//   
//   /* ADC1 Configuration *******************************************************/
//   /* ADCs DeInit */  
//   ADC_DeInit(ADC1);
//   
//   /* Configure the ADC1 in continous mode withe a resolutuion equal to 12 bits*/
//   ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
//   ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
//   ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_Rising;    
//   ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T3_TRGO;
//   ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
//   ADC_InitStructure.ADC_ScanDirection = ADC_ScanDirection_Upward;
//   ADC_Init(ADC1, &ADC_InitStructure); 
//   
//   /* Convert the ADC1 Channel 11 with 239.5 Cycles as sampling time */ 
//   ADC_ChannelConfig(ADC1, ADC_Channel_11 , ADC_SampleTime_28_5Cycles);   

//   /* ADC Calibration */
//   ADC_GetCalibrationFactor(ADC1);

//   /* Enable the auto delay feature */    
//   ADC_WaitModeCmd(ADC1, ENABLE); 
//   
//   /* Enable the Auto power off mode */
//   ADC_AutoPowerOffCmd(ADC1, ENABLE); 
//   
//   /* Enable ADCperipheral[PerIdx] */
//   ADC_Cmd(ADC1, ENABLE);     
//   
//   /* Wait the ADCEN falg */
//   while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_ADEN)); 
//   
//   /* TIM2 enable counter */
//   TIM_Cmd(TIM3, ENABLE);
//   
//   
// }

/************************ END OF FILE *****************************************/
