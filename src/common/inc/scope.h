/**
  ******************************************************************************
  * @file    scope.h
  * @author  PTO Team
  * @version V1.0.0
  * @date    24/3/2013
  * @brief   This file contains all the functions prototypes for the Scope firmware 
  *          library.
  ******************************************************************************
*/ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SCOPE_H
#define __SCOPE_H

/* Includes ------------------------------------------------------------------*/
/* Zadne includy zde nebudou!!!*/


typedef enum
{
	SCOPE_RISING = 0,
	SCOPE_FALLING
} SCOPE_TRIGGER_EDGE;

typedef enum
{
	SCOPE_NORMAL = 0,
	SCOPE_AUTO,
	SCOPE_SINGLE
} SCOPE_TRIGGER_MODE;

typedef enum
{
	SCOPE_IDLE = 0,
	SCOPE_SAMPLING,
	SCOPE_TRIGGER_WAIT,
	SCOPE_DONE,
	SCOPE_ERR
}SCOPE_STATE;

typedef enum
{
	SCOPE_BUFF_FREE = 0,
	SCOPE_BUFF_DATA,
	SCOPE_BUFF_ALMOST_FULL,
	SCOPE_BUFF_FULL
}SCOPE_BUFF_STATE;


typedef struct
{
  uint16_t * memory;												/* Pointer na pamet kam muze Osciloskop zapisovat */
  uint16_t size;														/* Velikost pameti pro osciloskop */														
  uint16_t triggerPointer;									/* Pointer vzorku na kterem nastel triger */
	uint16_t writePointer;
  uint16_t readPointer;
	uint16_t preTrigger;											/* Kolik vzorku se ma nechat pred trigger udalosti */
  SCOPE_BUFF_STATE state;
} SCOPE_Buffer;


typedef struct
{
  uint32_t SCOPE_samplingFrequency;      
	SCOPE_Buffer* p_SCOPE_buffer;  						/* Pointer na scope buffer */
  SCOPE_TRIGGER_EDGE SCOPE_triggerEdge;			/* Hrana triggetu */
	SCOPE_TRIGGER_MODE SCOPE_triggerMode;			/* Typ triggeru */
	uint16_t SCOPE_triggerLevel;							/* komparovaci uroven triggeru */
	SCOPE_STATE SCOPE_state;	
} SCOPE_InitTypeDef;


/* SCOPE_Exported_Functions */
void SCOPE_init(SCOPE_InitTypeDef * p_SCOPE_desc); /* Inicializace pomoci decsriptoru */
void SCOPE_set_sample_rate(uint32_t smpRate); /* Nastaveni rychlosi vzorkovani*/
uint16_t SCOPE_get_voltage (uint8_t samples);				/* Hodnota pro voltmeter */
void SCOPE_start_meas(void);
void SCOPE_stop_meas(void);
void SCOPE_DMA_meas(void);									/* Single mereni nejvyssi rychlosti */

void SCOPE_set_trigger_level(uint16_t trigLevel);
void SCOPE_set_trigger_mode(SCOPE_TRIGGER_MODE trigMode);
void SCOPE_set_trigger_edge(SCOPE_TRIGGER_EDGE trigEdge);

SCOPE_STATE SCOPE_get_state(void);					/* Vrati stav knihovny osciloskopu */
void SCOPE_ADC_request(void);								/* Kontrola triggeru a konce mereni pri kazdem odmerenem vzorku (volano z ADC)*/


#endif /*__SCOPE_H */

/************************ END OF FILE *****************************************/
