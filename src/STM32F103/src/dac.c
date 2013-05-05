/**
  ******************************************************************************
  * @file    dac.c
  * @author  PTO Team
  * @version V1.0.0
  * @date    3/3/2013
  * @brief   Tato knihovna se stara o obsluhu DAC (inicializace,sprava preruseni...)
  *          Moznosti generovani jsou:
  *           - generovani pomoci citace
  *           - generovani pomoci DMA
  *           - nastaveni jednoho vzorku (vyuzitelne pro DDS)                
  *       
  ******************************************************************************  
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "stm32f10x_dac.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_dma.h"
#include "dac.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/ 

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
bool DACunInitialized = TRUE;
DAC_STATE dacState = DAC_ERR;
uint16_t lDAC_memorySize;
//PTO_DAC_InitTypeDef DAC_desc;

uint16_t DACbufferPointer = 0;  // Ukazuje do bufferu na vzorek, ktery byl naposled nastaven


/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

u32 TIM_APB1_GetClockFreq(void){
	RCC_ClocksTypeDef	RCC_Clocks;

 	RCC_GetClocksFreq(&RCC_Clocks);
		
	if(RCC_Clocks.HCLK_Frequency == RCC_Clocks.PCLK1_Frequency){
		return	RCC_Clocks.PCLK1_Frequency; 
	}else{
		return	2*(RCC_Clocks.PCLK1_Frequency);
	}
}
/**
  * @brief  inicializace DAC
  * @param  DAC deskriptor
  * @retval None
  */
void DAC_init(PTO_DAC_InitTypeDef * p_DAC_desc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef    TIM_TimeBaseStructure;
	DAC_InitTypeDef            DAC_InitStructure;
	DMA_InitTypeDef            DMA_InitStructure;

//	RCC_ClocksTypeDef	RCC_Clocks;
 	
	s32 clk_div;
	u16 prescaler;
	u16 arr_reg;
	
//	RCC_GetClocksFreq(&RCC_Clocks);

	lDAC_memorySize = p_DAC_desc->DAC_memorySize;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  
  	/* Configure PA.04 (DAC_OUT1) as analog */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;

  	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* TIM2 Periph clock enable */
  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
  
  	/* Time base configuration */
  	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure); 
	//sampling_frequency = frequency / (psc*arr)
	//clk_div = psc*arr
	clk_div = TIM_APB1_GetClockFreq() / p_DAC_desc->DAC_samplingFrequency;
	
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

	TIM_TimeBaseStructure.TIM_Prescaler = prescaler;
	TIM_TimeBaseStructure.TIM_Period = arr_reg;	
	TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;    
  	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
  	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

	DAC_DMA_start();


  	/* TIM2 TRGO selection */
  	TIM_SelectOutputTrigger(TIM2, TIM_TRGOSource_Update);
  
  	/* TIM2 enable counter */
  	TIM_Cmd(TIM2, ENABLE);
	
	DAC_DeInit(); 
          
    /* DAC channel1 Configuration */
    DAC_InitStructure.DAC_Trigger = DAC_Trigger_T2_TRGO;  		//prepis na vystup podle TIM2
    DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;
          
					/* DMA channel2 Configuration */
   	DMA_DeInit(DMA1_Channel2); 
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&DAC->DHR12R1;
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)p_DAC_desc->p_DAC_memory;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
    DMA_InitStructure.DMA_BufferSize = p_DAC_desc->DAC_memorySize;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel2, &DMA_InitStructure);
					
			/* Enable DMA1 Channel2 */
    DMA_Cmd(DMA1_Channel2, ENABLE);
					
          	/* DAC Channel1 Init */
    DAC_Init(DAC_Channel_1, &DAC_InitStructure);

          	/* Enable DAC Channel1: Once the DAC channel1 is enabled, PA.04 is 
             automatically connected to the DAC converter. */
    DAC_Cmd(DAC_Channel_1, ENABLE);
	dacState = DAC_IDLE;
         
}

/**
  * @brief  Nastavi vzorek na vystup
  * @param  uint16_t - hodnota, ktera se ma nastavit (v bitech)
  * @retval None
  */
void DAC_set_sample(uint16_t sample)
{
	TIM_DMACmd(TIM2, TIM_DMA_Update, DISABLE);					//dma request podle TIM2
	//DAC_DHR12R1_ADDRESS
	DAC->DHR12R1 = sample;
  	dacState = DAC_RUN;
	
}        

/**
  * @brief  Odstartuje generovani pomoci DMA
  * @param  None
  * @retval None
  */
void DAC_DMA_start()
{
   /* Enable DMA for DAC Channel1 */
    TIM_DMACmd(TIM2, TIM_DMA_Update, ENABLE);					//dma request podle TIM2
	dacState = DAC_DMA_RUN;
}                

/**
  * @brief  Vrati stav DAC
  * @param  None
  * @retval DAC_STATE
  */
DAC_STATE DAC_get_state()
{
  return dacState;
}            

/**
  * @brief  Vrati index vzorku ktery byl nastaven
  * @param  None
  * @retval uint16_t - index do pole
  */
uint16_t DAC_get_pointer()
{
  return DACbufferPointer;
}              

/**
  * @brief  Nastavi pointer na vzorek o 1 pred ten, ktery se má generovat
  * @param  uint16_t - index
  * @retval int8_t - 0 ok, -1 chyba
  */
int8_t DAC_set_pointer(uint16_t pointer)
{
  int8_t result = -1;
  if (pointer < lDAC_memorySize){ /* Kontrola jestli index ukazuje do pole */ 
    DACbufferPointer = pointer;
    result = 0;
  }
  return result;
}     

/**
  * @brief  Obsluha preruseni (bud od casovace pri pravidelnem generovani 
  *         nebo od DMA aby se znovu spustilo)
  * @param  None
  * @retval None
  */
void DAC_IRQ_handler()
{
  //zavolani fce v pointeru
}                


/************************ END OF FILE *****************************************/
