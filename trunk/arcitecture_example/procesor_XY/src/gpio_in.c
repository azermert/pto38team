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
#include "<procesor>.h"
#include "gpio_in.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/ 
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
bool unInitialized = true;
bool overflowed = false;
GPIO_IN_STATE gpioInState = GPIO_ERR;
GPIO_IN_InitTypeDef GPIO_IN_desc;
uint16_t bufferPointer = 0;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Inicializace pomoci deskriptoru
  * @param  GPIO_IN_InitTypeDef
  * @retval None
  */    
void GPIO_IN_init(*GPIO_IN_InitTypeDef p_GPIO_IN_desc)
{
  //TODO
}     

/**
  * @brief  Vrati stav gpio_in knihovny
  * @param  None
  * @retval GPIO_IN_STATE
  */
GPIO_IN_STATE GPIO_IN_get_state()
{
  return gpioInState;
}            

/**
  * @brief  Precteni slova na vstupu
  * @param  None
  * @retval uint8_t - slovo na vstupu
  */
uint8_t GPIO_get_word()
{
  //TODO
}                   

/**
  * @brief  Vrati index do pole
  * @param  None
  * @retval uint16_t - index
  */
uint16_t GPIO_get_pointer()
{
  return bufferPointer
}                  

/**
  * @brief  Kontrola preteceni bufferu 
  * @param  None
  * @retval bool
  */
bool GPIO_is_buffer_overflowed()
{
  return overflowed;
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


/************************ END OF FILE *****************************************/
