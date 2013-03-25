/**
  ******************************************************************************
  * @file    counter.h
  * @author  PTO Team
  * @version V1.0.0
  * @date    24/3/2013
  * @brief   This file contains all the functions prototypes for the counter firmware 
  *          library.
  ******************************************************************************
*/ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __procesor_COUNTER_H
#define __procesor_COUNTER_H

/* Includes ------------------------------------------------------------------*/
/* Zadne includy zde nebudou!!!*/

typedef enum
{
	COUNT_IDLE = 0,
	COUNT_SAMPLING,
	COUNT_ERR
} COUNT_STATE;


/* COUNTER_Exported_Functions */
void COUNT_init(void); 											/* Inicializace citace bez delicky */
void COUNT_start(void);
void COUNT_stop(void);
COUNT_STATE COUNT_get_state(void);					
uint32_t COUNT_get_freq(void); 							/* Vrati (zmerenou frekvenci)*100 Hz */  
uint32_t COUNT_get_filtred_freq(void); 			/* Vrati (zmerenou a vyfiltrovanou frekvenci)*100 Hz */  
void COUNT_GPIO_EXT_reguest(void);					/* Prisel impuls k zapocitani */
     
#endif /*__<procesor>_COUNTER_H */

/************************ END OF FILE *****************************************/
