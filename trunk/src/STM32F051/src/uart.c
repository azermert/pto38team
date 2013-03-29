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
#include "stm32f0xx.h"
#include "comm.h"
#include "uart.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/ 
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
UART_InitTypeDef urt;

/* Private function prototypes -----------------------------------------------*/
void send_next(void);
/* Private functions ---------------------------------------------------------*/


/**
 * @brief  Ulozi char na vystupni buffer 
 * @param  char
 * @retval Buffer state
 */ 
BUFF_STATE store_byte(char chr)
{

  uint16_t tmpPointer;
	*(((*urt.p_inBuffer).memory)+((*urt.p_inBuffer).writePointer))=chr;
	tmpPointer=(*urt.p_inBuffer).writePointer+1;
	
	if (tmpPointer>((*urt.p_inBuffer).size-1))
	{
		tmpPointer =0;
	}
	
	if(tmpPointer == (*urt.p_inBuffer).readPointer)
	{
		(*urt.p_inBuffer).state = BUFF_FULL;
	}
	else
	{
		(*urt.p_inBuffer).writePointer=tmpPointer;
		(*urt.p_inBuffer).state = BUFF_DATA;
	}
	
	if(BUFF_FULL!=(*urt.p_inBuffer).state && (UART_BUFF_SIZE-UART_BUFF_ALMOST_FULL)>=COMM_get_in_free_space()){
		(*urt.p_inBuffer).state = BUFF_ALMOST_FULL;
	}

	return (*urt.p_inBuffer).state;
};  


/**
  * @brief  Inicializace pomoci decsriptoru
  * @param  Descriptor obsahujici parametry pro nastaveni UART
  * @retval None
  */  
void UART_init(UART_InitTypeDef * p_UART_desc)
{
	urt=*p_UART_desc;
	
	RCC->APB2ENR |= (RCC_APB2ENR_IOPAEN|RCC_APB2ENR_AFIOEN);		   /* APB2ENR = APB2 peripheral clock enable register*/
	
	GPIOA->CRH = 0x444444B4;//UART 9,10

	
 	RCC->APB2ENR |=  RCC_APB2ENR_USART1EN;

	USART1->CR1 |= 	USART_CR1_UE 		// UART Enable
					|USART_CR1_RE		// Recieve enable
					|USART_CR1_TE		// Transmit enable
					|USART_CR1_RXNEIE
					//|USART_CR1_IDLEIE
					//|USART_CR1_TXEIE
					;	
	USART1->BRR = 0x0D0;	
	// 24MHz/16 = 1500kHz >>> 1500kHz/115200Bd = 13,02 >>> matisa = D; fraction = 0 => 0x0D0
	
		NVIC->ISER[1] |= (1 << (USART1_IRQn & 0x1F));   // enable interrupt
};

/**
  * @brief  Fce viditelna z COMM urcena k odstartovani prenosu pokud jsou data v bufferu
  * @param  None
  * @retval None
  */ 
void UART_tick()
{
	send_next();
};  

/**
 * @brief  Obsluha preruseni UART
 * @param  None
 * @retval None
 */                     
void USART1_IRQHandler()
{
	volatile unsigned int StatusValue;

  StatusValue = USART1->SR;
	  
	if (StatusValue & USART_SR_RXNE)
	{                  
		store_byte(USART1->DR);
  }

  if (0 != (StatusValue & USART_SR_TXE))
	{
		send_next();
  }
};




void send_next(void)
{
	if((*urt.p_outBuffer).writePointer == (*urt.p_outBuffer).readPointer){		// buffer is empty
		//USART1->CR1 &= ~USART_CR1_TE;		
		USART1->CR1 &= ~USART_CR1_TXEIE;	// disable "TxEmpty register" interrupt		
		(*urt.p_outBuffer).state=BUFF_FREE;
		return;
	}	

	USART1->DR = *(((*urt.p_outBuffer).memory)+((*urt.p_outBuffer).readPointer));

	(*urt.p_outBuffer).readPointer += 1;

	if((*urt.p_outBuffer).readPointer > (UART_BUFF_SIZE-1))
	{
		(*urt.p_outBuffer).readPointer= 0;
	}

	if((*urt.p_outBuffer).readPointer == (*urt.p_outBuffer).writePointer)			// buffer is empty
	{
		//USART1->CR1 &= ~USART_CR1_TE;		
		USART1->CR1 &= ~USART_CR1_TXEIE;	// disable "TxEmpty register" interrupt		
		(*urt.p_outBuffer).state=BUFF_FREE;		
	}else{
	   USART1->CR1 |= USART_CR1_TXEIE;
	}
}


/************************ END OF FILE *****************************************/
