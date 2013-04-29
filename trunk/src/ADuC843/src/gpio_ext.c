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
#include "ADuC843.h"
#include "typedef.h"
#include "gpio_ext.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/ 
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
bool lGpioExtUnInitialized = TRUE;
GPIO_EXT_STATE lGpioExtState = GPIO_ERR;
GPIO_EXT_InitTypeDef lGpio_EXT_desc;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Inicializace pomoci deskriptoru
  * @param  GPIO_EXT_InitTypeDef
  * @retval None
  */    
void GPIO_EXT_init(GPIO_EXT_InitTypeDef *_desc)
{
  //TODO
}

/**
  * @brief  Zacne pocitat pulzy pomoci preruseni
  * @param  None
  * @retval None
  */  
void GPIO_EXT_start_counting()
{
  //TODO
}      

/**
  * @brief  Zastavi citani
  * @param  None
  * @retval None
  */  
void GPIO_EXT_stop_counting(void)
{
  //TODO
}           

/**
  * @brief  Vrati stav gpio_ext knihovny
  * @param  None
  * @retval GPIO_IN_STATE
  */
GPIO_EXT_STATE GPIO_EXT_get_state(void)
{
  //TODO
  return lGpioExtState;
}

/**
  * @brief  Preruseni od casovace k prectani vzorku a zavolani fce v pointeru
  * @param  None
  * @retval None
  */
void GPIO_COUNT_IRQ_handler(void)
{
  //zavolani fce v pointeru
}


/************************ END OF FILE *****************************************/
