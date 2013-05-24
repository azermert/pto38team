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
#include "stm32f30x.h"
#include "dac.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/ 
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
bool DACunInitialized = TRUE;
DAC_STATE dacState = DAC_ERR;
pto_DAC_InitTypeDef DAC_desc;


uint16_t DACbufferPointer = 0;  // Ukazuje do bufferu na vzorek, ktery byl naposled nastaven
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


/**
  * @brief  inicializace DAC
  * @param  DAC deskriptor
  * @retval None
  */
void DAC_initialize(pto_DAC_InitTypeDef * p_DAC_desc)
{
  DAC_InitTypeDef            DAC_InitStructure;
  DMA_InitTypeDef            DMA_InitStructure;
	GPIO_InitTypeDef 					 GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef    TIM_TimeBaseStructure;
	
	/* DMA2 clock enable (to be used with DAC) */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);

  /* DAC Periph clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);

  /* GPIOA clock enable */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
  
  /* Configure PA.04 (DAC_OUT1) as analog */
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/* TIM2 Periph clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
  
  /* Time base configuration */
  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure); 
  TIM_TimeBaseStructure.TIM_Period = p_DAC_desc->DAC_samplingFrequency;//0x8C9;          
  TIM_TimeBaseStructure.TIM_Prescaler = 0x00;       
  TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;    
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

  /* TIM2 TRGO selection */
  TIM_SelectOutputTrigger(TIM2, TIM_TRGOSource_Update);
  
  /* TIM2 enable counter */
  TIM_Cmd(TIM2, ENABLE);
	
	/* DAC channel2 Configuration */
	DAC_DeInit(); 
	DAC_InitStructure.DAC_Trigger = DAC_Trigger_T2_TRGO;
	DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
	DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude = DAC_LFSRUnmask_Bits11_0;
	DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Disable;
	/* DAC Channel2 Init */
	DAC_Init(DAC_Channel_1, &DAC_InitStructure);
	
	/* Enable DAC Channel2 */
	DAC_Cmd(DAC_Channel_1, ENABLE);
	
	
	/* DMA2 channel3 configuration */
	DMA_DeInit(DMA2_Channel3); 
	DMA_InitStructure.DMA_PeripheralBaseAddr = DAC_DHR12R1_ADDRESS;
	//DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&genSine;
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t) p_DAC_desc->p_DAC_memory;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
	DMA_InitStructure.DMA_BufferSize = p_DAC_desc->DAC_memorySize;//32;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA2_Channel3, &DMA_InitStructure);
	
	/* Enable DMA2 Channel3 */
	//DMA_Cmd(DMA2_Channel3, ENABLE);
	
	/* Enable DMA for DAC Channel2 */
	//DAC_DMACmd(DAC_Channel_1, ENABLE);
	dacState = DAC_IDLE;
}

/**
  * @brief  Nastavi vzorek na vystup
  * @param  uint16_t - hodnota, ktera se ma nastavit (v bitech)
  * @retval None
  */
void DAC_set_sample(uint16_t sample)
{
  //TODO
	dacState = DAC_RUN;
}        

/**
  * @brief  Odstartuje generovani pomoci DMA
  * @param  None
  * @retval None
  */
void DAC_DMA_start()
{
  /* Enable DMA2 Channel3 */
	DMA_Cmd(DMA2_Channel3, ENABLE);
	
	/* Enable DMA for DAC Channel2 */
	DAC_DMACmd(DAC_Channel_1, ENABLE);
	
	dacState = DAC_DMA_RUN;
}                

/**
  * @brief  Zastavi generovani pomoci DMA
  * @param  None
  * @retval None
  */
void DAC_DMA_stop()
{
  /* Enable DMA2 Channel3 */
	DMA_Cmd(DMA2_Channel3, DISABLE);
	
	/* Enable DMA for DAC Channel2 */
	DAC_DMACmd(DAC_Channel_1, DISABLE);
	
	dacState = DAC_IDLE;
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
  if (pointer < DAC_desc.DAC_memorySize){ /* Kontrola jestli index ukazuje do pole */ 
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
