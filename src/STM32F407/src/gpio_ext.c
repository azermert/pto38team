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
//#include "stm32f4.h"
#include "gpio_ext.h"

#include "stm32f4xx.h"
#include "stm32f4xx_syscfg.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_exti.h"
#include "misc.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/ 
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
bool unInitialized = true;
GPIO_EXT_STATE gpioExtState = GPIO_ERR;
GPIO_EXT_InitTypeDef GPIO_EXT_desc;

bool working = false; // probiha citani?
EXTI_InitTypeDef   EXTI_InitStructure;
uint

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Inicializace pomoci deskriptoru
  * @param  GPIO_EXT_InitTypeDef
  * @retval None
  */    
void GPIO_EXT_init(struct GPIO_IN_InitTypeDef *p_GPIO_IN_desc)
{
	
	
	GPIO_InitTypeDef   GPIO_InitStructure;
  NVIC_InitTypeDef   NVIC_InitStructure;

	//!!! nebude se inicializovat uz nekde drive??
  /* CLK pro GPIOA*/
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  /* Enable SYSCFG clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
  
  /* Plovouci vystup na PA0 */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* Prirazeni EXTI k PA0 */
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);

  /* Config EXTI Line0 */
  EXTI_InitStructure.EXTI_Line = EXTI_Line0;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);

  /* Enable and set EXTI Line0 Interrupt to the lowest priority */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
	unInitialized = false;
	//TODO - jak vypada *p_GPIO_IN_desc?? - ted je nastaveny jeden konkretni pin (PA0) s nejnizsi prioritou
}

/**
  * @brief  Zacne pocitat pulzy pomoci preruseni
  * @param  None
  * @retval None
  */  
void GPIO_EXT_start_counting()
{
  working = true;
	
	//TODO
}      

/**
  * @brief  Zastavi citani
  * @param  None
  * @retval None
  */  
void GPIO_EXT_stop_counting()
{
  working = true;
}           

/**
  * @brief  Vrati stav gpio_ext knihovny
  * @param  None
  * @retval GPIO_IN_STATE
  */
GPIO_EXT_STATE GPIO_EXT_get_state()
{
  //TODO
  return gpioExtState;
}

/**
  * @brief  Preruseni od casovace k prectani vzorku a zavolani fce v pointeru
  * @param  None
  * @retval None
  */


void GPIO_COUNT_IRQ_handler() 
{
		//!!! z ktereho pointeru??
	//zavolani fce v pointeru
}

void EXTI0_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line0) != RESET)
  {
    if (working) {
			GPIO_COUNT_IRQ_handler();  
    }
		
		
		
		/* Smazni priznakovy bit preruseni EXTI */
    EXTI_ClearITPendingBit(EXTI_Line0);
  }
}

/*
hint
http://www.coocox.org/show_exam/EXTI/444.html
*/


/************************ END OF FILE *****************************************/
