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
#ifndef __<procesor>_UART_H
#define __<procesor>_UART_H

/* Includes ------------------------------------------------------------------*/
/* Zadne includy zde nebudou!!!*/

typedef void (*tick_comm)(void);

typedef struct
{
  uint32_t baudrate;
  struct COMM_Buffer * p_inBuffer;
  struct COMM_Buffer * p_outBuffer;
  tick_comm p_COMM_tick;
  UART_STATE state;     
}UART_InitTypeDef;


typedef enum
{
	UART_OK = 0,
	UART_ERR
}UART_STATE;


/* UART_Exported_Functions */
void UART_init(struct UART_InitTypeDef * p_UART_desc);        /* Inicializace pomoci decsriptoru */
void UART_tick();                         /* Fce viditelna z COMM urcena k odstartovani prenosu pokud jsou data v bufferu*/
//void UART_IRQ_handler();


#endif /*__<procesor>_UART_H */

/************************ END OF FILE *****************************************/
