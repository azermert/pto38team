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
#ifndef __LOG_ANALYSER_H
#define __LOG_ANALYSER_H

/* Includes ------------------------------------------------------------------*/
/* Zadne includy zde nebudou!!!*/

#define LA_CH_0                 ((uint16_t)0x0001)  /*!< Pin 0 selected    */
#define LA_CH_1                 ((uint16_t)0x0002)  /*!< Pin 1 selected    */
#define LA_CH_2                 ((uint16_t)0x0004)  /*!< Pin 2 selected    */
#define LA_CH_3                 ((uint16_t)0x0008)  /*!< Pin 3 selected    */
#define LA_CH_4                 ((uint16_t)0x0010)  /*!< Pin 4 selected    */
#define LA_CH_5                 ((uint16_t)0x0020)  /*!< Pin 5 selected    */
#define LA_CH_6                 ((uint16_t)0x0040)  /*!< Pin 6 selected    */
#define LA_CH_7                 ((uint16_t)0x0080)  /*!< Pin 7 selected    */

typedef enum
{
	LA_TRIG_AND=0,
	LA_TRIG_OR
} LA_TRIGGER_LOGIC;


typedef enum
{
	LA_RISING = 0,
	LA_FALLING
} LA_TRIGGER_EDGE;



typedef enum
{
	LA_IDLE = 0,
	LA_SAMPLING,
	LA_TRIGGER_WAIT,
	LA_DONE,
	LA_ERR
}LA_STATE;

typedef enum
{
	LA_BUFF_FREE = 0,
	LA_BUFF_DATA,
	LA_BUFF_FULL
} LA_BUFF_STATE;


typedef struct
{
  uint16_t * sampleMemory;									/* Pointer na pamet kam muze log analyzator zapisovat vzorky*/
	uint16_t * repeatMemory;									/* Pointer na pamet kam LA zapisuje pocty opakovani vzorku */
  uint16_t size;														/* Velikost pameti pro osciloskop */														
	uint16_t writePointer;
	LA_BUFF_STATE state;
} LA_Buffer;


typedef struct
{
  uint32_t LA_samplingFrequency;      
	LA_Buffer* p_LA_buffer;  									/* Pointer na LA buffer */
  LA_TRIGGER_EDGE LA_triggerEdge;						/* Hrana triggetu */
	uint8_t LA_triggerChannles;								/* Typ triggeru */
	LA_TRIGGER_LOGIC LA_triggerLogic;					/* Logicka funkce spousteni triggeru */
	LA_STATE LA_state;	
} LA_InitTypeDef;


/* LA_Exported_Functions */
void LA_init(LA_InitTypeDef * p_LA_desc); 	/* Inicializace pomoci decsriptoru */
void LA_set_sample_rate(uint32_t smpRate); 	/* Nastaveni rychlosi vzorkovani*/

void LA_start_meas(void);
void LA_stop_meas(void);

void LA_set_trigger_channel(LA_TRIGGER_LOGIC trigLog, uint8_t trigChan);
void LA_set_trigger_edge(LA_TRIGGER_EDGE trigEdge);

LA_STATE LA_get_state(void);								/* Vrati stav knihovny osciloskopu */
void LA_GPIO_IN_request(void);							/* Kontrola triggeru a konce mereni pri kazdem odmerenem vzorku (volano z GPIO_IN)*/


#endif /*__LOG_ANALYSER_H */

/************************ END OF FILE *****************************************/
