/**
  ******************************************************************************
  * @file    counter.c
  * @author  PTO Team
  * @version V1.0.0
  * @date    24/3/2013
  * @brief   Knihovna pro citac. Mereni probiha stylem cas mezi pusly s tim,
	*						ze k mereni casu je pouzit timer bez delicky			
  ******************************************************************************  
  */ 

/* Includes ------------------------------------------------------------------*/
//#include "<procesor>.h"
#include "typedefs.h"
#include "counter.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/ 
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

uint32_t frequency=0;
uint32_t filtredFreq=0;
uint32_t numOfCountOverflows=0;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


/**
 * @brief  Inicializace citace na co nejvetsi rychlost
 * @param  None
 * @retval None
 */    
void COUNT_init()
{
	//TODO
}

/**
 * @brief  Odstartuje mereni (timer)
 * @param  None
 * @retval None
 */ 
void COUNT_start(void)
{
	//TODO
}

/**
 * @brief  Zastavi mereni
 * @param  None
 * @retval None
 */ 
void COUNT_stop(void)
{
	//TODO
}

/**
 * @brief  Vrati stav citace
 * @param  None
 * @retval state
 */ 
COUNT_STATE COUNT_get_state(void)
{
	//TODO
	return COUNT_ERR;
}

/**
 * @brief  Vrati (zmerenou frekvenci)*100 Hz
 * @param  None
 * @retval None
 */    
uint32_t COUNT_get_freq(void)    
{
	//TODO
	return frequency;
}

/**
 * @brief  Vrati zmerenou a vyfiltrovanou frekvenci * 100 Hz
 * @param  None
 * @retval None
 */    
uint32_t COUNT_get_filtred_freq(void) 
{
	//TODO
	return filtredFreq;
}

/**
 * @brief   Prisel impuls k zapocitani
 * @param  None
 * @retval None
 */    
void COUNT_GPIO_EXT_reguest()
{
	//TODO
}

/**
 * @brief  Preruseni od preteceni citace
 * @param  None
 * @retval None
 */    
void TIM_IRQ_handler()
{
	//TODO
}

/************************ END OF FILE *****************************************/
