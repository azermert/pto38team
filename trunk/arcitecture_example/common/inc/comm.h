/**
  ******************************************************************************
  * @file    comm.h
  * @author  PTO Team
  * @version V1.0.0
  * @date    3/3/2013
  * @brief   ToDo
  ******************************************************************************
*/ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __COMM_H
#define __COMM_H

/* Includes ------------------------------------------------------------------*/
//#include "scpi.h" 
//#include "uart.h"
/* Zadne includy zde nebudou!!!*/

typedef void (*tick_adc)(void);

typedef struct
{
  uint16_t bufferSize;        
  COMM_SPEED speed;
}COMM_InitTypeDef;

typedef struct
{
  uint32_t * memory;
  uint16_t size;
  uint16_t inPointer;
  uint16_t outPointer;
  BUFF_STATE state;
}COMM_Buffer;


enum BUFF_STATE{
	BUFF_FREE = 0,
	BUFF_ALMOST_FULL,
	BUFF_FULL
};

enum COMM_SPEED{
	COMM_SAFE_MODE = 0,
	COMM_NORMAL,
	COMM_HIGH
};

enum COMM_STATE{
	COMM_IDLE = 0,
	COMM_RUN,
	COMM_ERR
};

typedef struct
{
  SCPI_CMD command;
  SCPI_PARAM param1;
  int32_t data1;
  SCPI_PARAM param2;
  int32_t data2;
  SCPI_PARAM param3;
  int32_t data3;
  bool answer
}COMM_CMD;


/* COMM_Exported_Functions */
void COMM_init(*COM_InitTypeDef);         /* Inicializace pomoci decsriptoru */
BUFF_STATE COMM_get_in_buff_state();      /* Vrati stav prichoziho bufferu */
BUFF_STATE COMM_get_out_buff_state();     /* Vrati stav odchoziho bufferu */
uitn16_t COMM_get_in_free_space();        /* Vrati pocet volnych byte v prichozim bufferu */
uitn16_t COMM_get_out_free_space();       /* Vrati pocet volnych byte v odchozim bufferu */
COMM_STATE COMM_get_state();              /* Vrati stav knihovny */

void COMM_put_char(char c);               /* Ulozi char na vystupni buffer */
void COMM_put_char(uint8_t c);            /* Ulozi znak na vystupni buffer */
void COMM_send(uint32_t *,uint16_t);      /* Prekopiruje zadanou pamet na vystupni buffer */

uint16_t COMM_read(uint32_t *,uint16_t);  /* Prekopiruje vstupni buffer na pridelenou pamet a vrati kolik skutence bylo prekopirovano */
uint8_t COMM_read_char();                 /* Precte znak z prichoziho bufferu*/

COMM_CMD* COMM_get_command();             /* Zavola scpi/nasi knihovnu na rozpoznani prikazu a vrati prikaz z vstupniho bufferu*/
void COMM_tick();                         /* Uart zada o obsluhu (plny buffer/chyba atd) */   

#endif /*__COMM_H */

/************************ END OF FILE *****************************************/
