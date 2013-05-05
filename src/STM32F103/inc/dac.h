/**
  ******************************************************************************
  * @file    dac.h
  * @author  PTO Team
  * @version V1.0.0
  * @date    3/3/2013
  * @brief   This file contains all the functions prototypes for the DAC firmware 
  *          library.
  ******************************************************************************
*/ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __stm32f0xx_DAC_H
#define __stm32f0xx_DAC_H

/* Includes ------------------------------------------------------------------*/
/* Zadne includy zde nebudou!!!*/

typedef void (*tick_dac)(void);

typedef struct
{
  uint32_t DAC_samplingFrequency;        
  uint16_t * p_DAC_memory;                /* Pointer na pamet, ze ktere DAC muze cist */ 
  uint32_t DAC_memorySize;                /* Velikost pameti pro DAC */ 
}PTO_DAC_InitTypeDef;


typedef enum
{
	DAC_IDLE = 0,
	DAC_RUN,
	DAC_DMA_RUN,
	DAC_ERR
}DAC_STATE;


/* ADC_Exported_Functions */
void DAC_init(PTO_DAC_InitTypeDef * p_DAC_desc);          /* Inicializace pomoci decsriptoru */
void DAC_set_sample(uint16_t sample);     /* Nastavi hodnotu na vystup */
void DAC_DMA_start(void);                     /* Odstartuje generovani vystupu pomoci DMA */
DAC_STATE DAC_get_state(void);                /* Vrati stav prevodniku */
uint16_t DAC_get_pointer(void);               /* Vrati pozici posledniho nastaveneho vzorku */
int8_t DAC_set_pointer(uint16_t pointer); /* Nastavi odkud se ma zacit generovat */
//void ADC_IRQ_handler();                 /* Obsluha preruseni */



#endif /*__stm32f0xx_DAC_H */

/************************ END OF FILE *****************************************/
