/**
  ******************************************************************************
  * @file    scope.h
  * @author  PTO Team
  * @version V1.0.0
  * @date    24/3/2013
  * @brief   This file contains all the functions prototypes for the Generator firmware 
  *          library.
  ******************************************************************************
*/ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GENERATOR_H
#define __GENERATOR_H

/* Includes ------------------------------------------------------------------*/
/* Zadne includy zde nebudou!!!*/


typedef enum
{
	GEN_NONE =0,
	GEN_SQUARE,
	GEN_TRIANGLE,
	GEN_SINE,
	GEN_ARB
} GEN_SIGNAL_TYPE;



typedef enum
{
	GEN_IDLE = 0,
	GEN_RUN,
	GEN_ERR
}GEN_STATE;


typedef struct
{
  uint16_t * memory;												/* Pointer na pamet kam muze Generator zapisovat sve signaly*/
  uint16_t size;														/* Velikost pameti pro generator */														
  uint16_t readPointer;
} GEN_Buffer;

typedef struct
{
	GEN_SIGNAL_TYPE GEN_signalType;						/* Typ signalu ke generovani*/
	uint16_t	amplitude;											/* Amplituda signalu */
	uint16_t  offset;													/* Offset signalu (stejnosmerny posun bez oz na vystupu)*/
	uint16_t  duty;														/* Strida signalu */
	uint32_t	frequency;											/* (Frekvence generovaneho signalu)*1000 Hz*/
} GEN_SIGNAL;

typedef struct
{
  	GEN_Buffer* p_GEN_buffer;									/* Pointer na pamet bufferu */
	uint32_t	GEN_sampleRate;									/* Rychlost generovani */	
	GEN_SIGNAL*	p_GEN_signal;									/* Pointer na structuru signalu */
	GEN_STATE GEN_state;											/* stav generatoru */
} GEN_InitTypeDef;

#ifndef ADUC843
#define GEN_BUFF_LENGHT 500
extern GEN_SIGNAL	gGenSignal;
#endif

/* GEN_Exported_Functions */
void GEN_init(GEN_InitTypeDef * p_GEN_desc); /* Inicializace pomoci decsriptoru */
bool GEN_set_signal(GEN_SIGNAL* _sig);/* Do bufferu napocita vzorky pro dany signal, frekvenci offset atd*/

void GEN_start(void);													/* Odstartuje generovani signalu */
void GEN_stop(void);														/* Zastavi generovani signalu */

GEN_STATE GEN_get_state(void);									/* Vrati stav generatoru */


#endif /*__GENERATOR_H */

/************************ END OF FILE *****************************************/
