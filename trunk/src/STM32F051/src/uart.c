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
#include "stm32f0xx_conf.h"
#include "uart.h"

#include "NVIC_Basic.h"
// #include "stm32f0xx_rcc.c"
// #include "stm32f0xx_gpio.c"
// #include "stm32f0xx_usart.c"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/ 
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
PTO_UART_InitTypeDef urt;
NVIC_InitTypeDef invDef;


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
	GPIO_InitTypeDef GPIO_InitStructure;
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
  //GPIO_PinAFConfig(GPIOA, GPIO_PinSource | GPIO_PinSource15, GPIO_AF_1);
  //GPIO_PinAFConfig(GPIOA, GPIO_PinSource15, GPIO_AF_1);


	//GPIO_DeInit(GPIOA);
	//Configure USART1 pins:  Rx and Tx ----------------------------
  // Rx
/*	 GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	 GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
 	// Tx
 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_Init(GPIOA, &GPIO_InitStructure);*/
	 
	    /* Connect PXx to USARTx_Tx */
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_1);

    /* Connect PXx to USARTx_Rx */
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_1);
    
    /* Configure USART Tx, Rx as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &GPIO_InitStructure); 
	 
	 
 }
 
void UART_init(PTO_UART_InitTypeDef * _desc)
{
	USART_InitTypeDef USART_InitStruct;

	urt=*_desc;

	USART_StructInit(&USART_InitStruct);	 	//8bit, parity_none, stopbit_1 ..

	USART_InitStruct.USART_BaudRate = _desc->baudrate;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);

	USART_DeInit(USART1);
	USART_Init(USART1, &USART_InitStruct);

	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	
	
	invDef.NVIC_IRQChannel = USART1_IRQn;
	invDef.NVIC_IRQChannelPriority = 3;
	invDef.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&invDef);
		
	initHW_UART();
	USART_Cmd(USART1, ENABLE);
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
void USART1_IRQHandler(void)
{
	volatile unsigned int StatusValue;

  	//StatusValue = USART1->ISR;

if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
  {
    /* Read one byte from the receive data register */
                  
		store_byte(USART1->RDR);
  	}

  	if (USART_GetITStatus(USART1, USART_IT_TXE) != RESET)
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

	if(USART1->ISR & USART_ISR_TXE){

		USART1->TDR = *(((*urt.p_outBuffer).memory)+((*urt.p_outBuffer).readPointer));
	
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
}

/************************ END OF FILE *****************************************/
