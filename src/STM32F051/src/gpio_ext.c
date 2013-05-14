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
#include "stm32f0xx.h"
#include "typedefs.h"
#include "gpio_ext.h"
#include "stm32f0xx_conf.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/ 
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
bool GPIOEXTunInitialized = TRUE;
GPIO_EXT_STATE gpioExtState = GPIO_ERR;
GPIO_EXT_InitTypeDef GPIO_EXT_desc;

EXTI_InitTypeDef   EXTI_InitStructure;
bool preruseni = FALSE;
uint8_t state = 0;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Inicializace pomoci deskriptoru
  * @param  GPIO_EXT_InitTypeDef
  * @retval None
  */   


void GPIO_EXT_init(GPIO_EXT_InitTypeDef *p_GPIO_EXT_desc)
{
	EXTI_InitTypeDef   EXTI_InitStructure;
  GPIO_InitTypeDef   GPIO_InitStructure;
  NVIC_InitTypeDef   NVIC_InitStructure;
  
  /* Enable GPIOA clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

  /* Configure PA0 pin as input floating */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* Enable SYSCFG clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
  
  /* Connect EXTI0 Line to PA0 pin */
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);

  /* Configure EXTI0 line */
  EXTI_InitStructure.EXTI_Line = EXTI_Line0;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);

  /* Enable and set EXTI0 Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI0_1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 0x00;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	GPIOEXTunInitialized = FALSE;
}

/**
  * @brief  Zacne pocitat pulzy pomoci preruseni
  * @param  None
  * @retval None
  */  
void GPIO_EXT_start_counting()
{
  preruseni = TRUE;
}      

/**
  * @brief  Zastavi citani
  * @param  None
  * @retval None
  */  
void GPIO_EXT_stop_counting()
{
  preruseni = FALSE;
}           

/**
  * @brief  Vrati stav gpio_ext knihovny
  * @param  None
  * @retval GPIO_IN_STATE
  */
GPIO_EXT_STATE GPIO_EXT_get_state()
{
  gpioExtState = state;
	state = 0;
	
  return gpioExtState;
}

/**
  * @brief  Preruseni od casovace k prectani vzorku a zavolani fce v pointeru
  * @param  None
  * @retval None
  */
void GPIO_COUNT_IRQ_handler()
{
  state = state + 1;
}

void EXTI0_1_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line0) != RESET)
  {
    if (preruseni) {
			GPIO_COUNT_IRQ_handler();  
    };

    /* Clear the EXTI line 0 pending bit */
    EXTI_ClearITPendingBit(EXTI_Line0);
  }
}


/************************ END OF FILE *****************************************/
