/**
  ******************************************************************************
  * @file    gpio_out.c
  * @author  PTO Team - Jakub Halcin
  * @version V1.0.0
  * @date    11/3/2013
  * @brief   Tato knihovna obsluhuje vystupni GPIO piny  - pracovni verze         
  *       
  ******************************************************************************  
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_conf.h"
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
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_38 | GPIO_Pin_39 | GPIO_Pin_40 | GPIO_Pin_41 | GPIO_Pin_42 | GPIO_Pin_43 | GPIO_Pin_44 | GPIO_Pin_45;  //PE7-PE14
	GPIO_Setup.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
		
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP; 	// this sets the pin type to push / pull (as opposed to open drain)
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL; 	// this sets the pullup / pulldown resistors to be inactive
	GPIO_Init(GPIOE, &GPIO_InitStruct);
 	GPIOE_OTYPER &= 0xFF00;
//AS IS
}                       

/**
  * @brief  Nastavi na vystup dane kodove slovo
  * @param  uint8_t - kodove slovo
  * @retval None
  */
void GPIO_set_word(uint8_t word)
{
 	settedValue= word;
	GPIOB_ODR = settedValue;
	return;
}          

/**
  * @brief  Vrati hodnotu, ktera byla skutecne nastavena
  * @param  None
  * @retval uint8_t - nastavena hodnota
  */
uint8_t GPIO_get_setted_val()
{
  
  return settedValue;
}      


/************************ END OF FILE *****************************************/
