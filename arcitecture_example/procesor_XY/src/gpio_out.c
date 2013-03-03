/**
  ******************************************************************************
  * @file    gpio_out.c
  * @author  PTO Team
  * @version V1.0.0
  * @date    3/3/2013
  * @brief   Tato knihovna obsluhuje vystupni GPIO piny           
  *       
  ******************************************************************************  
  */ 

/* Includes ------------------------------------------------------------------*/
#include "<procesor>.h"
#include "gpio_out.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/ 
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
bool unInitialized = true;
uint8_t settedValue = 0;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Inicializace
  * @param  None
  * @retval None
  */
void GPIO_OUT_init()
{
  //TODO
}                       

/**
  * @brief  Nastavi na vystup dane kodove slovo
  * @param  uint8_t - kodove slovo
  * @retval None
  */
void GPIO_set_word(uint8_t word)
{
  //TODO
}          

/**
  * @brief  Vrati hodnotu, ktera byla skutecne nastavena
  * @param  None
  * @retval uint8_t - nastavena hodnota
  */
uint8_t GPIO_get_setted_val()
{
  //TODO
}      


/************************ END OF FILE *****************************************/
