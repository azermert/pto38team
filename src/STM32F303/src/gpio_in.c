/**
  ******************************************************************************
  * @file    gpio_in.c
  * @author  PTO Team
  * @version V1.0.0
  * @date    3/3/2013
  * @brief   Tato knihovna obsluhuje vstupni GPIO piny 
  *          (slouzi hlavne pro logicky analyzator - pro usetreni pameti 
  *           bude log analyzator pouzivat RLE kodovani)            
  *       
  ******************************************************************************  
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm32f30x.h"
#include "gpio_in.h"
#include "comm.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/ 
#define DATA_SIZE			2048
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
bool lGPIOInUnInitialized = TRUE;
bool lGPIOInOverflowed = FALSE;
GPIO_IN_STATE lGpioInState = GPIO_ERR;
GPIO_IN_InitTypeDef lGPIO_IN_desc;
uint16_t lGPIOInBufferPointer = 0;


/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Inicializace pomoci deskriptoru
  * @param  GPIO_IN_InitTypeDef
  * @retval None
  */    
void GPIO_IN_init(GPIO_IN_InitTypeDef * _desc)
{
  	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHBPeriphClockCmd(PORTS_CLOCKS, ENABLE);

	GPIO_InitStructure.GPIO_Pin = DATA0_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;	
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(DATA0_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = DATA1_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(DATA1_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = DATA2_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(DATA2_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = DATA3_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(DATA3_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = DATA4_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(DATA4_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = DATA5_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(DATA5_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = DATA6_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(DATA6_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = DATA7_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(DATA7_PORT, &GPIO_InitStructure);
	
	GPIO_trigger_init(_desc);
}     

/**
  * @brief  Vrati stav gpio_in knihovny
  * @param  None
  * @retval GPIO_IN_STATE
  */
GPIO_IN_STATE GPIO_IN_get_state()
{
  return lGpioInState;
}            

/**
  * @brief  Precteni slova na vstupu
  * @param  None
  * @retval uint8_t - slovo na vstupu
  */
uint8_t GPIO_get_word()
{
	uint8_t ret;
	uint16_t p = GPIO_ReadInputData(GPIOD);
	
	p = p & 0xFF00;
	ret = p >> 8;
	
  return ret;
}                   

/**
  * @brief  Vrati index do pole
  * @param  None
  * @retval uint16_t - index
  */
uint16_t GPIO_get_pointer()
{
  return lGPIOInBufferPointer;
}                  

/**
  * @brief  Kontrola preteceni bufferu 
  * @param  None
  * @retval bool
  */
bool GPIO_is_buffer_overflowed()
{
  return lGPIOInOverflowed;
}          

/**
  * @brief  Preruseni od casovace k prectani vzorku a zavolani fce v pointeru
  * @param  None
  * @retval None
  */
void GPIO_LOG_IRQ_handler()
{
  //zavolani fce v pointeru
}

uint32_t GPIO_TIM_APB1_GetClockFrequency(void){
	RCC_ClocksTypeDef	RCC_Clocks;

 	RCC_GetClocksFreq(&RCC_Clocks);
		
	if(RCC_Clocks.HCLK_Frequency == RCC_Clocks.PCLK1_Frequency){
		return	RCC_Clocks.PCLK1_Frequency; 
	}else{
		return	2*(RCC_Clocks.PCLK1_Frequency);
	}
}

void GPIO_trigger_init(GPIO_IN_InitTypeDef * _desc){
	TIM_TimeBaseInitTypeDef 	TIM_TimeBaseInitStruct;
	TIM_OCInitTypeDef	TIM_OCInitStructure;
	NVIC_InitTypeDef 		NVIC_InitStructure;
	s32 clk_div;
	u16 prescaler;
	u16 arr_reg;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	
	TIM_DeInit(TRIGGER_TIMER);
	TIM_TimeBaseStructInit(&TIM_TimeBaseInitStruct);

	//sampling_frequency = frequency / (psc*arr)
	//clk_div = psc*arr
	clk_div = GPIO_TIM_APB1_GetClockFrequency() / _desc->GPIO_IN_samplingFrequency;
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

	TIM_ARRPreloadConfig(TRIGGER_TIMER, ENABLE);						

	TIM_OCStructInit(&TIM_OCInitStructure);

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 0x01;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
  TIM_OC4Init(TRIGGER_TIMER, &TIM_OCInitStructure);
	
		/* Enable the TIM4 gloabal Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = TRIGGER_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
	TIM_ITConfig(TRIGGER_TIMER, TIM_IT_CC4, ENABLE);
	TIM_ClearITPendingBit(TRIGGER_TIMER, TIM_IT_CC4);
	
	TIM_Cmd(TRIGGER_TIMER, ENABLE);

}

void TIM4_IRQHandler()
{
	static uint8_t i;
	static uint16_t tmp;
	
	/*
		GPIO_IDLE = 0,
	GPIO_RUN,
	GPIO_ERR
	*/
	
	switch(lGpioInState){
		case GPIO_RUN:

			tmp = GPIO_get_word();
/*
			if(lGPIOInBufferPointer == lBuffSize){
				lGPIOInBufferPointer = 0;
			}
*/
			COMM_put_char((uint8_t)tmp);
		
			break;


	   	case GPIO_IDLE:
			case GPIO_ERR:
	   	default:
	   		break;
	}//switch

	

	//NVIC_ClearPending(TRIGGER_IRQn);	
	TIM_ClearITPendingBit(TRIGGER_TIMER, TIM_IT_CC4);
	
	if(i%2==0){
		GPIOE->BSRR = GPIO_Pin_10;
	} else {
		GPIOE->BRR = GPIO_Pin_10;
	}
	i++;
}

/************************ END OF FILE *****************************************/
