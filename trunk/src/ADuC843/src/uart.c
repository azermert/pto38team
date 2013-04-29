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
#include "ADuC843.h"
#include "typedef.h"
#include "uart.h"

#include "NVIC_Basic.h"


sbit LED=P3^4;
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/ 
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
PTO_UART_InitTypeDef urt;

COMM_Buffer* inBuff;
COMM_Buffer* outBuff;
uint8_t sending=0;

//sbit LED=P3^4;
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
	LED=0;
	*(inBuff->memory + inBuff->writePointer)=chr;
	tmpPointer=inBuff->writePointer+1;
	
	if (tmpPointer>(inBuff->size_buff-1))
	{
		tmpPointer =0;
	}
	
	if(tmpPointer == inBuff->readPointer)
	{
		inBuff->state = BUFF_FULL;
	}
	else
	{
		inBuff->writePointer=tmpPointer;
		inBuff->state = BUFF_DATA;
	}
	
	if(BUFF_FULL!=inBuff->state && (UART_BUFF_SIZE-UART_BUFF_ALMOST_FULL)>=COMM_get_in_free_space()){
		inBuff->state = BUFF_ALMOST_FULL;
	}
LED=1;
	return inBuff->state;
}  


/**
  * @brief  Inicializace pomoci decsriptoru
  * @param  Descriptor obsahujici parametry pro nastaveni UART
  * @retval None
  */  
 

void UART_init(PTO_UART_InitTypeDef * _desc)
{
	urt=*_desc;
	inBuff=_desc->p_inBuffer;
	outBuff=_desc->p_outBuffer;
	
	SCON = 0x52; //nastaveni UART
	if(urt.baudrate == UART_NORMAL){
		T3CON= 0x83; //nastaveni baudrate 115200
		T3FD = 0x09;
	}else{
		T3CON= 0x86; //nastaveni baudrate 9600
		T3FD = 0x2D;
	}
	
	ES=1; //povoleni preruseni UART
		
	//LED=0;
	
/*	USART_InitTypeDef USART_InitStruct;

	

	USART_StructInit(&USART_InitStruct);	 	//8bit, parity_none, stopbit_1 ..

	USART_InitStruct.USART_BaudRate = _desc->baudrate;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);


	USART_DeInit(USART2);
	USART_Init(USART2, &USART_InitStruct);

	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	NVIC_IntEnable(USART2_IRQn,0x02);

	initHW_UART();
	USART_Cmd(USART2, ENABLE);*/
}

/**
  * @brief  Fce viditelna z COMM urcena k odstartovani prenosu pokud jsou data v bufferu
  * @param  None
  * @retval None
  */ 
void UART_tick()
{
	//send_next();
	if((*urt.p_outBuffer).writePointer != (*urt.p_outBuffer).readPointer && TI==0 && sending==0){
		TI=1;
	}
	
}  

/**
 * @brief  Obsluha preruseni UART
 * @param  None
 * @retval None
 */                     
void USART2_IRQHandler(void) interrupt 4
{
	
	/*volatile unsigned int StatusValue;

  	StatusValue = USART2->SR;
	  
	if (StatusValue & USART_SR_RXNE)
	{                  
		store_byte(USART2->DR);
  	}

  	if (0 != (StatusValue & USART_SR_TXE))
	{
		send_next();
  	}*/
	
	EA=0;
	if (RI == 1)
	{                  
		store_byte(SBUF);
		RI=0;
  	}

  	if (TI == 1)
	{
		sending=0;
		send_next();
		//TI=0;
  	}
	EA=1;
}




void send_next(void)
{
	
	/*if((*urt.p_outBuffer).writePointer == (*urt.p_outBuffer).readPointer){		// buffer is empty
		//USART1->CR1 &= ~USART_CR1_TE;		
		USART2->CR1 &= ~USART_CR1_TXEIE;	// disable "TxEmpty register" interrupt		//kdyz neni buffer k odeslani prazdny, nechase zapnute interpt
		(*urt.p_outBuffer).state=BUFF_FREE;
		return;
	}	

	if(USART2->SR & USART_SR_TXE){

		USART2->DR = *(((*urt.p_outBuffer).memory)+((*urt.p_outBuffer).readPointer)); //SBUF
	
		(*urt.p_outBuffer).readPointer += 1;
	
		if((*urt.p_outBuffer).readPointer > (UART_BUFF_SIZE-1))
		{
			(*urt.p_outBuffer).readPointer= 0;
		}
	
		if((*urt.p_outBuffer).readPointer == (*urt.p_outBuffer).writePointer)			// buffer is empty
		{
			//USART1->CR1 &= ~USART_CR1_TE;		
			USART2->CR1 &= ~USART_CR1_TXEIE;	// disable "TxEmpty register" interrupt		
			(*urt.p_outBuffer).state=BUFF_FREE;		
		}else{
		   USART2->CR1 |= USART_CR1_TXEIE;
		}
	}*/
	
	if((*urt.p_outBuffer).writePointer == (*urt.p_outBuffer).readPointer){		// buffer is empty?
		//USART1->CR1 &= ~USART_CR1_TE;		
		//ES=0;	// disable "TxEmpty register" interrupt		
		(*urt.p_outBuffer).state=BUFF_FREE;
		TI=0;
		return;
	}

	if(TI == 1){
		//while(TI==0);
		TI=0;
		SBUF = *((outBuff->memory)+(outBuff->readPointer)); //SBUF
		sending=1;
		
		outBuff->readPointer += 1;
	
		if(outBuff->readPointer > (UART_BUFF_SIZE-1))
		{
			outBuff->readPointer= 0;
		}
	
		if(outBuff->readPointer == outBuff->writePointer)			// buffer is empty
		{
			//USART1->CR1 &= ~USART_CR1_TE;		
			//ES=0;	// disable "TxEmpty register" interrupt		
			outBuff->state=BUFF_FREE;	
			TI=0;			
		}else{
		   //ES=1;
		}
	}
}


/************************ END OF FILE *****************************************/
