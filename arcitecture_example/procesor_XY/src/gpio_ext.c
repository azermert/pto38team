/**
  ******************************************************************************
  * @file    gpio_ext.c
  * @author  PTO Team
  * @version V1.0.0
  * @date    3/3/2013
  * @brief   Tato knihovna obsluhuje vstupni GPIO piny obsluhovane prerusenim
  *          (slouzi hlavne pro citac)            
  *       
  ******************************************************************************  
  */ 

/* Includes ------------------------------------------------------------------*/
#include "<procesor>.h"
#include "gpio_ext.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/ 
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
bool unInitialized = true;
GPIO_IN_STATE gpioInState = GPIO_ERR;
GPIO_IN_InitTypeDef GPIO_IN_desc;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Inicializace pomoci deskriptoru
  * @param  GPIO_EXT_InitTypeDef
  * @retval None
  */    
void GPIO_EXT_init(*GPIO_IN_InitTypeDef)
{
  //TODO
}

/**
  * @brief  Vrati stav gpio_ext knihovny
  * @param  None
  * @retval GPIO_IN_STATE
  */
GPIO_EXT_STATE GPIO_EXT_get_state()
{
  //TODO
}

/**
  * @brief  Preruseni od casovace k prectani vzorku a zavolani fce v pointeru
  * @param  None
  * @retval None
  */
void GPIO_COUNT_IRQ_handler()
{
  //zavolani fce v pointeru
}


/************************ END OF FILE *****************************************/
