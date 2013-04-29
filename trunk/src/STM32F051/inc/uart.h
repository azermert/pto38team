/**
  ******************************************************************************
  * @file    uart.h
  * @author  PTO Team
  * @version V1.0.0
  * @date    3/3/2013
  * @brief   This file contains all the functions prototypes for the UART firmware 
  *          library. 
  ******************************************************************************
*/ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __stm32f0xx_UART_H
#define __stm32f0xx_UART_H


/* Includes ------------------------------------------------------------------*/
#include "comm.h"
/* Zadne includy zde nebudou!!!*/

#define UART_BUFF_SIZE 1024
#define UART_BUFF_ALMOST_FULL (3*UART_BUFF_SIZE)/4

typedef void (*tick_comm)(void);

typedef enum
{
	UART_OK = 0,
	UART_ERR
}UART_STATE;

typedef enum                              
{
	UART_SAFE_MODE = 9600,                 
	UART_NORMAL	= 115200,
	UART_HIGH                                                 
}UART_SPEED;

typedef struct _PTO_UART_InitTypeDef
{
  UART_SPEED baudrate;
  COMM_Buffer* p_inBuffer;
  COMM_Buffer* p_outBuffer;
  tick_comm p_COMM_tick;
  UART_STATE state;     
}PTO_UART_InitTypeDef;


/* UART_Exported_Functions */
void UART_init(PTO_UART_InitTypeDef * p_UART_desc);        /* Inicializace pomoci decsriptoru */
void UART_tick(void);                         /* Fce viditelna z COMM urcena k odstartovani prenosu pokud jsou data v bufferu*/
//void USART1_IRQHandler(void);


#endif /*__stm32f0xx_UART_H */

/************************ END OF FILE *****************************************/
