/**
  ******************************************************************************
  * @file    uart.c
  * @author  PTO Team
  * @version V1.0.0
  * @date    11/3/2013
  * @brief   Zajistuje plneni prichoziho bufferu a odesilani odchoziho bufferu.
  *          UART_tick() - podpurna fce pro COMM, ktera tim rika uartu, 
  *          ze ma data k odeslani a uz je na uartu jak je odesle (DMA/IRQ)          
  *       
  ******************************************************************************  
  */ 

/* Includes ------------------------------------------------------------------*/
//#include "<procesor>.h"
#include "stm32f10x.h"
#include "comm.h"
#include "uart.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/ 
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/



/**
  * @brief  Inicializace pomoci decsriptoru
  * @param  Descriptor obsahujici parametry pro nastaveni UART
  * @retval None
  */  
void UART_init(UART_InitTypeDef * p_UART_desc)
{
  //TODO
};

/**
  * @brief  Fce viditelna z COMM urcena k odstartovani prenosu pokud jsou data v bufferu
  * @param  None
  * @retval None
  */ 
void UART_tick()
{
  //TODO
};  

/**
 * @brief  Obsluha preruseni UART
 * @param  None
 * @retval None
 */                     
void UART_IRQ_handler()
{
  //TODO
};


/************************ END OF FILE *****************************************/
