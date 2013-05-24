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
#include "stm32f30x.h"
#include "uart.h"

#include "NVIC_Basic.h"
#include "stm32f30x_rcc.h"
#include "stm32f30x_gpio.h"
#include "stm32f30x_usart.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/ 
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
PTO_UART_InitTypeDef urt;

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
	
	if (tmpPointer>((*urt.p_inBuffer).size_buff-1))
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
 void initHW_UART(void){
	GPIO_InitTypeDef	GPIO_InitStructure;

	/* Enable GPIO clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOD, ENABLE);

  /* Enable USART clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE); 

  /* Connect PXx to USARTx_Tx */
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource8, GPIO_AF_7);

  /* Connect PXx to USARTx_Rx */
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource9, GPIO_AF_7);
  
  /* Configure USART Tx as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
    
  /* Configure USART Rx as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_Init(GPIOD, &GPIO_InitStructure);


}

void UART_initialize(PTO_UART_InitTypeDef * _desc)
{
	USART_InitTypeDef USART_InitStruct;

	urt=*_desc;

	USART_StructInit(&USART_InitStruct);	 	//8bit, parity_none, stopbit_1 ..

	USART_InitStruct.USART_BaudRate = _desc->baudrate;
	//RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);


	USART_DeInit(USART3);
	USART_Init(USART3, &USART_InitStruct);

	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);

	NVIC_IntEnable(USART3_IRQn,0x02);

	initHW_UART();
	USART_Cmd(USART3, ENABLE);
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
void USART3_IRQHandler(void)
{
	volatile unsigned int StatusValue;

  	StatusValue = USART3->ISR;
	  
	if (StatusValue & USART_ISR_RXNE)
	{                  
		store_byte(USART3->RDR);
  	}

  	if (0 != (StatusValue & USART_ISR_TXE))
	{
		send_next();
  	}
};




void send_next(void)
{
	if((*urt.p_outBuffer).writePointer == (*urt.p_outBuffer).readPointer){		// buffer is empty
		//USART1->CR1 &= ~USART_CR1_TE;		
		USART3->CR1 &= ~USART_CR1_TXEIE;	// disable "TxEmpty register" interrupt		
		(*urt.p_outBuffer).state=BUFF_FREE;
		return;
	}	

	if(USART3->ISR & USART_ISR_TXE){

		USART3->TDR = *(((*urt.p_outBuffer).memory)+((*urt.p_outBuffer).readPointer));
	
		(*urt.p_outBuffer).readPointer += 1;
	
		if((*urt.p_outBuffer).readPointer > (UART_BUFF_SIZE-1))
		{
			(*urt.p_outBuffer).readPointer= 0;
		}
	
		if((*urt.p_outBuffer).readPointer == (*urt.p_outBuffer).writePointer)			// buffer is empty
		{
			//USART1->CR1 &= ~USART_CR1_TE;		
			USART3->CR1 &= ~USART_CR1_TXEIE;	// disable "TxEmpty register" interrupt		
			(*urt.p_outBuffer).state=BUFF_FREE;		
		}else{
		   USART3->CR1 |= USART_CR1_TXEIE;
		}
	}
}


/************************ END OF FILE *****************************************/
