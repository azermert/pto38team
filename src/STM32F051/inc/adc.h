/**
  ******************************************************************************
  * @file    adc.h
  * @author  PTO Team
  * @version V1.0.0
  * @date    3/3/2013
  * @brief   This file contains all the functions prototypes for the ADC firmware 
  *          library.
  ******************************************************************************
*/ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __stm32f0xx_ADC_H
#define __stm32f0xx_ADC_H

/* Includes ------------------------------------------------------------------*/
/* Zadne includy zde nebudou!!!*/

typedef void (*tick_adc)(void);

typedef struct
{
  uint32_t ADC_samplingFrequency;        
  uint32_t * p_ADC_memory;                /* Pointer na pamet kam muze ADC zapisovat */ 
  uint32_t ADC_memorySize;                /* Velikost pameti pro ADC */ 
  tick_adc p_ADC_tick;                    /* Pointer na funkci OSC_ADC_Request() */
}ADC_InitTypeDef;


typedef enum
{
	ADC_IDLE = 0,
	ADC_RUN_INF,
	ADC_DMA_RUN,
	ADC_DMA_DONE,
	ADC_ERR
}ADC_STATE;


/* ADC_Exported_Functions */
void ADC_init(ADC_InitTypeDef * p_ADC_desc);          /* Inicializace pomoci decsriptoru */
uint16_t ADC_meas_sample(void);               /* Zmeri jeden vzorek a vrati vysledek */
void ADC_circle_meas_start(void);             /* Odstartuje nepretrzite mereni */
void ADC_circle_meas_stop(void);              /* Zastavi nepretrzite mereni */
void ADC_DMA_meas_start(void);                /* Odstartuje jednorazove mereni pomoci DMA */
ADC_STATE ADC_get_state(void);                /* Vrati stav prevodniku */
uint16_t ADC_get_pointer(void);               /* Vrati pozici posledniho nacteneho vzorku */
int8_t ADC_set_last_read(uint16_t pointer);               /* Nastavi index pole, ktery byl naposled precten/zpracovan */
bool ADC_is_buffer_overflowed(void);          /* Vrati zda-li buffer pri neustalem plneni pretekl nebo ne */
//void ADC_IRQ_handler();


#endif /*__stm32f0xx_ADC_H */

/************************ END OF FILE *****************************************/
