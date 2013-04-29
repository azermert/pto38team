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
#include "stm32f0xx.h"
#include "stm32f0xx_conf.h"
#include "gpio_out.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/ 
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
bool unInitialized = TRUE;
uint8_t settedValue = 0;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Inicializace
  * @param  None
  * @retval None
  */
	GPIO_InitTypeDef        GPIO_InitStructure;
	
void GPIO_OUT_init()
{

  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
 	// PC0 - PC7
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | 
																GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6	| GPIO_Pin_7; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
	unInitialized = FALSE;
	} 
                        
/**
  * @brief  Nastavi na vystup dane kodove slovo
  * @param  uint8_t - kodove slovo
  * @retval None
  */
void GPIO_set_word(uint8_t word)
{
  GPIOC->ODR = word;
}          

/**
  * @brief  Vrati hodnotu, ktera byla skutecne nastavena
  * @param  None
  * @retval uint8_t - nastavena hodnota
  */
uint8_t GPIO_get_setted_val()
{
	settedValue = ((uint8_t)GPIOC->IDR);
  return settedValue;
} 

/************************ END OF FILE *****************************************/
