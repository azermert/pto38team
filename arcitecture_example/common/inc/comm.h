/**
  ******************************************************************************
  * @file    comm.h
  * @author  PTO Team
  * @version V1.0.0
  * @date    3/3/2013
  * @brief   This file contains all the functions prototypes for the COMM firmware 
  *          library.   
  ******************************************************************************
*/ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __COMM_H
#define __COMM_H

/* Includes ------------------------------------------------------------------*/
#include "scpi.h" /* jedina vyjimka includu v headru, protoze scpi.h nebude nikde jinde potreba */
/* Zadne includy zde nebudou!!!*/

//pokus moje;

typedef void (*tick_comm)(void);

/**
 * Kvuli ruznemu HW u studentu. Nekdo ma USB prevodnik, 
 * nekdo jen MAX323 na nepajivem poli, ktere 921600 baud/s nepřenese
*/
typedef enum                              
{
	COMM_SAFE_MODE = 0,                      /* 9600 baud/s */
	COMM_NORMAL,                             /* 115200 baud/s */
	COMM_HIGH                                /* 921600 baud/s */
}COMM_SPEED;

typedef enum
{
	BUFF_FREE = 0,
	BUFF_ALMOST_FULL,
	BUFF_FULL
}BUFF_STATE;

typedef enum
{
	COMM_IDLE = 0,
	COMM_RUN,
	COMM_ERR
}COMM_STATE;

typedef struct
{     
  COMM_SPEED speed;
}COMM_InitTypeDef;

typedef struct
{
  uint8_t * memory;
  uint16_t size;
  uint16_t writePointer;
  uint16_t readPointer;
  BUFF_STATE state;
} COMM_Buffer;

typedef struct
{
  SCPI_CMD command;
  SCPI_PARAM param1;
  int32_t data1;
  SCPI_PARAM param2;
  int32_t data2;
  SCPI_PARAM param3;
  int32_t data3;
  bool answer;
} COMM_CMD;


/* COMM_Exported_Functions */
void COMM_init(COMM_InitTypeDef * p_COMM_desc);         /* Inicializace pomoci decsriptoru */
BUFF_STATE COMM_get_in_buff_state(void);      /* Vrati stav prichoziho bufferu */
BUFF_STATE COMM_get_out_buff_state(void);     /* Vrati stav odchoziho bufferu */
uint16_t COMM_get_in_free_space(void);        /* Vrati pocet volnych byte v prichozim bufferu */
uint16_t COMM_get_out_free_space(void);       /* Vrati pocet volnych byte v odchozim bufferu */
COMM_STATE COMM_get_state(void);              /* Vrati stav knihovny */

BUFF_STATE COMM_put_char(char chr);               /* Ulozi char na vystupni buffer */
BUFF_STATE COMM_put_uchar(uint8_t chr);            /* Ulozi znak na vystupni buffer */
int8_t COMM_send(uint32_t * memory, uint16_t size);      /* Prekopiruje zadanou pamet na vystupni buffer */

int16_t COMM_read(uint32_t * memory, uint16_t size);  /* Prekopiruje vstupni buffer na pridelenou pamet a vrati kolik skutence bylo prekopirovano */
int16_t COMM_read_char(void);                 /* Precte znak z prichoziho bufferu*/

COMM_CMD* COMM_get_command(void);     /* Zavola scpi/nasi knihovnu na rozpoznani prikazu a vrati prikaz z vstupniho bufferu*/
void COMM_tick(void);                         /* Uart zada o obsluhu (plny buffer/chyba atd) */   

#endif /*__COMM_H */

/************************ END OF FILE *****************************************/
