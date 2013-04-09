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
	TRIG_SIGNAL = 0,
	TRIG_SW_AUTO
} SCOPE_TRIGGER_MODE;

typedef enum
{
	SCOPE_IDLE = 0,			//NOP
	SCOPE_SAMPLING,			//getting data after trigger
	SCOPE_TRIGGER_WAIT,		//getting data before trigger
	SCOPE_DONE,
	SCOPE_ERR
}SCOPE_STATE;

typedef enum
{
	SCOPE_BUFF_FREE = 0,
	SCOPE_BUFF_DATA,
	//SCOPE_BUFF_ALMOST_FULL,
	SCOPE_BUFF_FULL
}SCOPE_BUFF_STATE;


typedef struct
{
  	uint16_t * memory;												/* Pointer na pamet kam muze Osciloskop zapisovat */
  	uint16_t size;														/* Velikost pameti pro osciloskop */														
	uint16_t triggerIndex;									/* Pointer vzorku na kterem nastel triger */
	uint16_t writeIndex;		//last write
  	uint16_t readIndex;
	uint16_t preTrigger;
	uint16_t dataRemain;	  	//post-trigger data counter	| 0 = done
	uint16_t indexStart;
	volatile SCOPE_BUFF_STATE state;
} SCOPE_Buffer;


typedef struct
{
  	uint32_t 			SCOPE_samplingFrequency;      
	SCOPE_Buffer* 		p_SCOPE_buffer;  						/* Pointer na scope buffer */
  	SCOPE_TRIGGER_EDGE 	SCOPE_triggerEdge;			/* Hrana triggetu */
	SCOPE_TRIGGER_MODE 	SCOPE_triggerMode;			/* Typ triggeru */
	uint16_t 			SCOPE_triggerLevel;							/* komparovaci uroven triggeru */
	SCOPE_STATE 		SCOPE_state;	
} SCOPE_TypeDef;

extern SCOPE_TypeDef gSCOPE;

/* SCOPE_Exported_Functions */
void SCOPE_init(SCOPE_TypeDef * p_SCOPE_desc); /* Inicializace pomoci decsriptoru */
void SCOPE_set_sample_rate(uint32_t smpRate); /* Nastaveni rychlosi vzorkovani*/
uint16_t SCOPE_get_voltage (uint8_t samples);				/* Hodnota pro voltmeter */
void SCOPE_start_meas(void);
void SCOPE_stop_meas(void);
void SCOPE_DMA_meas(void);									/* Single mereni nejvyssi rychlosti */

void SCOPE_set_trigger_level(uint16_t trigLevel);
void SCOPE_set_trigger_mode(SCOPE_TRIGGER_MODE trigMode);
void SCOPE_set_trigger_edge(SCOPE_TRIGGER_EDGE trigEdge);
void SCOPE_SW_trigger(void);
SCOPE_STATE SCOPE_get_state(void);
void SCOPE_setPreTrigger(float _value);					/* Vrati stav knihovny osciloskopu */
//void SCOPE_ADC_request(void);								/* Kontrola triggeru a konce mereni pri kazdem odmerenem vzorku (volano z ADC)*/


#endif /*__SCOPE_H */

/************************ END OF FILE *****************************************/
