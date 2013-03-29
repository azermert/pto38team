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
#include "adc.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/ 
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
bool unInitialized = TRUE;
bool ADCoverflowed = FALSE;
ADC_STATE adcState = ADC_ERR;
ADC_InitTypeDef ADC_desc;

uint16_t ADCbufferPointer = 0;  // Ukazuje do bufferu na vzorek, ktery byl naposled precten
uint16_t ADCbufferLastRead = 0;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/




/**
  * @brief  Inicializace AD prevodniku
  * @param  Descriptor obsahujici parametry pro nastaveni ADC
  * @retval None
  */
void ADC_init(ADC_InitTypeDef * p_ADC_desc)
{
  //TODO
} 

/**
  * @brief  Vrati hodnotu namereneho vzorku (bud z pole, ktere postupne plni nebo odstartuje jendo mereni)
  * @param  None
  * @retval Namereni Vzorek
  */         
uint16_t ADC_meas_sample()
{
  //TODO
  return 0;
}           

/**
  * @brief  Odstartuje nepretrzite mereni a plneni bufferu do kruhu (pomoci casovace)
  * @param  None
  * @retval None
  */
void ADC_circle_meas_start()
{
  //TODO
}

/**
  * @brief  Zastavi nepretrzite mereni
  * @param  None
  * @retval None
  */
void ADC_circle_meas_stop()
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
void ADC_DMA_meas_start()
{
  //TODO
}           

/**
  * @brief  Vrati stav AD prevodniku
  * @param  None
  * @retval ADC_STATE - Stav ADC
  */
ADC_STATE ADC_get_state()
{
  return adcState;
}            

/**
  * @brief  Vrati ukazetel do bufferu na posledni namereny vzorek
  * @param  None
  * @retval uint16_t - index pole
  */
uint16_t ADC_get_pointer()
{
  return ADCbufferPointer;
}               

/**
  * @brief  Nastavi index pole, ktery byl naposled precten/zpracovan
  * @param  uint16_t - index pole
  * @retval int8_t - 0 vse ok, -1 chyba
  */
int8_t ADC_set_last_read(uint16_t pointer)
{
  int8_t result = -1;
  if (pointer < ADC_desc.ADC_memorySize){ /* Kontrola jestli index ukazuje do pole */ 
    ADCbufferLastRead = pointer;
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
  return ADCoverflowed;
}          

/**
  * @brief  Zpracovani preruseni
  * @param  None
  * @retval None
  */
void ADC_IRQ_handler()
{
  //zavolani fce v pointeru
}

/************************ END OF FILE *****************************************/
