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
#include "dac.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/ 
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
bool DACunInitialized = TRUE;
DAC_STATE dacState = DAC_ERR;
DAC_InitTypeDef DAC_desc;

uint16_t DACbufferPointer = 0;  // Ukazuje do bufferu na vzorek, ktery byl naposled nastaven
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


/**
  * @brief  inicializace DAC
  * @param  DAC deskriptor
  * @retval None
  */
void DAC_init(DAC_InitTypeDef * p_DAC_desc)
{
  //TODO
}

/**
  * @brief  Nastavi vzorek na vystup
  * @param  uint16_t - hodnota, ktera se ma nastavit (v bitech)
  * @retval None
  */
void DAC_set_sample(uint16_t sample)
{
  //TODO
}        

/**
  * @brief  Odstartuje generovani pomoci DMA
  * @param  None
  * @retval None
  */
void DAC_DMA_start()
{
  //TODO
}                

/**
  * @brief  Vrati stav DAC
  * @param  None
  * @retval DAC_STATE
  */
DAC_STATE DAC_get_state()
{
  return DAC_ERR;
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
