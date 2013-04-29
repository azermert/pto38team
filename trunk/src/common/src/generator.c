/**
  ******************************************************************************
  * @file    generator.c
  * @author  PTO Team
  * @version V1.0.0
  * @date    24/3/2013
  * @brief   Knihovna zajistujici funkci generatoru. Obstarava vytvareni signalu a obsluhu DAC
  ******************************************************************************  
  */ 

/* Includes ------------------------------------------------------------------*/
//#include "<procesor>.h"
#include "typedefs.h"
#include "generator.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/ 
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
 * @brief   Inicializace pomoci decsriptoru
 * @param  GEN desc
 * @retval None
 */ 
void GEN_init(GEN_InitTypeDef * p_GEN_desc)
{
	//TODO
}

/**
 * @brief  Do bufferu napocita vzorky pro dany signal, frekvenci offset atd
 * (pokud se jedna o arb signal, tak nedela nic)
 * @param  GEN_signal
 * @retval Pokud se povedlo nastavit tak true
 */ 
bool GEN_set_signal(GEN_SIGNAL* p_GEN_signal)
{
	//TODO
	return FALSE;
}

/**
 * @brief  Odstartuje generovani signalu
 * @param  None
 * @retval None
 */ 
void GEN_start()													
{
	//TODO
}

/**
 * @brief  Zastavi generovani signalu 
 * @param  None
 * @retval None
 */ 
void GEN_stop()
{
	//TODO
}

/**
 * @brief  Vrati stav generatoru
 * @param  Gen state
 * @retval None
 */ 
GEN_STATE GEN_get_state()
{
	//TODO
	return GEN_ERR;
}



/************************ END OF FILE *****************************************/
