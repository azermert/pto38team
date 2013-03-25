/**
  ******************************************************************************
  * @file    comm.c
  * @author  PTO Team
  * @version V1.0.0
  * @date    11/3/2013
  * @brief   Komunikacni knihovna zajistujici inicializaci UART nebo USB a 
  *     funkce typu send, read a spravu bufferu. 
  *     Bude scpi knihovnu zadat o parsovani prikazu
  *     COMM_tick() - podpurna fce pro uart, ktery pomoci ni muze zadat obsluhu (napr plny buffer)  
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
COMM_Buffer outBuffer;
COMM_Buffer inBuffer;
COMM_InitTypeDef COMM_desc;
COMM_STATE commState;
COMM_CMD commCommand;
UART_InitTypeDef uart;



uint8_t outBuffField[UART_BUFF_SIZE];
uint8_t inBuffField[UART_BUFF_SIZE];

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
 * @brief  Inicializace pomoci decsriptoru
 * @param  desc
 * @retval None
 */ 
void COMM_init(COMM_InitTypeDef * p_COMM_desc)
{
	COMM_desc = *p_COMM_desc;
	
	outBuffer.size = UART_BUFF_SIZE;
	outBuffer.memory = &outBuffField[0];
	
  outBuffer.writePointer = 0;
  outBuffer.readPointer = 0;
  outBuffer.state = BUFF_FREE;
	
	inBuffer.size = UART_BUFF_SIZE;
	inBuffer.memory = &inBuffField[0];
	
  inBuffer.writePointer = 0;
  inBuffer.readPointer = 0;
  inBuffer.state = BUFF_FREE;
	
	switch(COMM_desc.speed)
	{
		case COMM_HIGH:uart.baudrate = UART_HIGH;break;
		case COMM_NORMAL:uart.baudrate = UART_NORMAL;break;
		default:uart.baudrate = UART_SAFE_MODE;
	}
	
	uart.p_inBuffer = &inBuffer;
	uart.p_outBuffer = &outBuffer;
	uart.p_COMM_tick = &COMM_tick;
	
	UART_init(&uart);

}        

/**
 * @brief  Vrati stav prichoziho bufferu 
 * @param  None
 * @retval Buffer state
 */ 
BUFF_STATE COMM_get_in_buff_state()
{
  return inBuffer.state;
};    

/**
 * @brief  Vrati stav odchoziho bufferu
 * @param  None
 * @retval Buffer state
 */ 
BUFF_STATE COMM_get_out_buff_state()
{
	if(outBuffer.writePointer == outBuffer.readPointer)
	{
		outBuffer.state = BUFF_FREE;
	}
	else
	{
		outBuffer.state = BUFF_DATA;
	}
	
	if(BUFF_FULL!=outBuffer.state && (UART_BUFF_SIZE-UART_BUFF_ALMOST_FULL)>=COMM_get_out_free_space()){
		outBuffer.state = BUFF_ALMOST_FULL;
	}
  return outBuffer.state;
};     

/**
 * @brief  Vrati pocet byte v prichozim bufferu 
 * @param  None
 * @retval uint16_t - free space
 */ 
uint16_t COMM_get_bytes_available() 
{
  return UART_BUFF_SIZE-COMM_get_in_free_space();
};      

/**
 * @brief  Vrati pocet byte v prichozim bufferu 
 * @param  None
 * @retval uint16_t - free space
 */ 
uint16_t COMM_get_in_free_space() 
{
	uint16_t result;
	if (inBuffer.readPointer>inBuffer.writePointer)
	{
		result = inBuffer.readPointer-inBuffer.writePointer;
	}
	else
	{
		result = inBuffer.size-inBuffer.writePointer+inBuffer.readPointer;
	}
  return result;
};

/**
 * @brief  Vrati pocet volnych byte v odchozim bufferu 
 * @param  None
 * @retval uint16_t - free space
 */ 
uint16_t COMM_get_out_free_space()
{

	uint16_t result;
	if (outBuffer.readPointer>outBuffer.writePointer)
	{
		result = outBuffer.readPointer-outBuffer.writePointer;
	}
	else
	{
		result = outBuffer.size-outBuffer.writePointer+outBuffer.readPointer;
	}
  return result;
};      

/**
 * @brief  Vrati stav knihovny
 * @param  None
 * @retval Comm state
 */ 
COMM_STATE COMM_get_state()
{
  //TODO
  return commState;
};             

/**
 * @brief  Ulozi char na vystupni buffer 
 * @param  char
 * @retval Buffer state
 */ 
BUFF_STATE COMM_put_char(char chr)
{

  uint16_t tmpPointer;
	
	outBuffField[outBuffer.writePointer]=chr;
	tmpPointer=outBuffer.writePointer+1;
	
	if (tmpPointer>(outBuffer.size-1))
	{
		tmpPointer =0;
	}
	
	
	if(tmpPointer == outBuffer.readPointer)
	{
		outBuffer.state = BUFF_FULL;
	}
	else
	{
		outBuffer.writePointer=tmpPointer;
		outBuffer.state = BUFF_DATA;
	}
	
	if(BUFF_FULL!=outBuffer.state && (UART_BUFF_SIZE-UART_BUFF_ALMOST_FULL)>=COMM_get_out_free_space()){
		outBuffer.state = BUFF_ALMOST_FULL;
	}
	return outBuffer.state;

};            

/**
 * @brief  Ulozi znak na vystupni buffer
 * @param  uchar
 * @retval Buffer state
 */ 
BUFF_STATE COMM_put_uchar(uint8_t chr)
{
  return COMM_put_char(chr);
};   

/**
 * @brief  Ulozi string na vystupni buffer
 * @param  string
 * @retval Buffer state
 */ 
BUFF_STATE COMM_print(char *chr)
{
	BUFF_STATE stat;
	while(*chr){
		stat=COMM_put_char(*chr);
		chr++;
	}
	UART_tick();
	return stat;
}; 

/**
 * @brief  Prekopiruje zadanou pamet na vystupni buffer 
 * @param  None
 * @retval ok = 0; error = -1
 */ 
int8_t COMM_send(uint8_t * memory, uint16_t size)
{
	uint16_t i;
  if(COMM_get_out_free_space()>=size)
	{
		for (i = 0;i<size;i++){
			COMM_put_char(*memory);
			memory++;
		}
		UART_tick();
		return 0;
	}
	else
	{
		return -1;
	}
};  

/**
 * @brief  Prekopiruje vstupni buffer na pridelenou pamet a vrati kolik skutence bylo prekopirovano
 * @param  None
 * @retval None
 */ 
int16_t COMM_read(uint8_t * memory, uint16_t size)
{
	uint16_t i=0;
	int16_t chr;
	while (-1!=chr && i<size)
	{
		chr = COMM_read_char();
		if (-1!=chr){
			*memory=chr;
			memory++;
			i++;
		}
	}
	return i;
};

/**
 * @brief  Precte znak z prichoziho bufferu
 * @param  None
 * @retval None
 */ 
int16_t COMM_read_char()
{
	
	int16_t result;
	
	if(inBuffer.writePointer == inBuffer.readPointer)
	{
		result = -1;
	}
	else
	{
		result = inBuffField[inBuffer.readPointer];
		inBuffer.readPointer++;
	}
	
	if(inBuffer.readPointer >= (inBuffer.size))
	{
		inBuffer.readPointer = 0; 
	}
	
	
  return result;
};            

/**
 * @brief  Zavola scpi/nasi knihovnu na rozpoznani prikazu a vrati prikaz z vstupniho bufferu
 * @param  None
 * @retval None
 */ 
COMM_CMD* COMM_get_command()
{
  //TODO
  return &commCommand;
}; 

/**
 * @brief  Uart zada o obsluhu (plny buffer/chyba atd)
 * @param  None
 * @retval None
 */ 
void COMM_tick()
{
  //TODO
};                    


/************************ END OF FILE *****************************************/
