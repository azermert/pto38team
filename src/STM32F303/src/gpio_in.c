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
#include "stm32f30x.h"
#include "gpio_in.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/ 
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
bool lGPIOInUnInitialized = TRUE;
bool lGPIOInOverflowed = FALSE;
GPIO_IN_STATE lGpioInState = GPIO_ERR;
GPIO_IN_InitTypeDef lGPIO_IN_desc;
uint16_t lGPIOInBufferPointer = 0;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Inicializace pomoci deskriptoru
  * @param  GPIO_IN_InitTypeDef
  * @retval None
  */    
void GPIO_IN_init(GPIO_IN_InitTypeDef * _desc)
{
  	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(PORTS_CLOCKS, ENABLE);

	GPIO_InitStructure.GPIO_Pin = DATA0_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;	
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(DATA0_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = DATA1_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(DATA1_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = DATA2_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(DATA2_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = DATA3_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(DATA3_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = DATA4_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(DATA4_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = DATA5_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(DATA5_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = DATA6_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(DATA6_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = DATA7_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(DATA7_PORT, &GPIO_InitStructure);
}     

/**
  * @brief  Vrati stav gpio_in knihovny
  * @param  None
  * @retval GPIO_IN_STATE
  */
GPIO_IN_STATE GPIO_IN_get_state()
{
  return lGpioInState;
}            

/**
  * @brief  Precteni slova na vstupu
  * @param  None
  * @retval uint8_t - slovo na vstupu
  */
uint8_t GPIO_get_word()
{
	uint8_t ret;
	uint16_t p = GPIO_ReadInputData(GPIOD);
	
	p = p & 0xFF00;
	ret = p >> 8;
	
  return ret;
}                   

/**
  * @brief  Vrati index do pole
  * @param  None
  * @retval uint16_t - index
  */
uint16_t GPIO_get_pointer()
{
  return lGPIOInBufferPointer;
}                  

/**
  * @brief  Kontrola preteceni bufferu 
  * @param  None
  * @retval bool
  */
bool GPIO_is_buffer_overflowed()
{
  return lGPIOInOverflowed;
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
