/**
  ******************************************************************************
  * @file    gpio_in.h
  * @author  PTO Team
  * @version V1.0.0
  * @date    3/3/2013
  * @brief   This file contains all the functions prototypes for the GPIO_in firmware 
  *          library.
  ******************************************************************************
*/ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __<procesor>_GPIO_IN_H
#define __<procesor>_GPIO_IN_H

/* Includes ------------------------------------------------------------------*/
/* Zadne includy zde nebudou!!!*/

typedef void (*tick_gpio_out)(void);

typedef struct
{
  uint32_t GPIO_IN_samplingFrequency;        
  uint32_t * p_GPIO_IN_memory;            /* Pointer na pamet kam muze GPIO zapisovat */ 
  uint32_t GPIO_IN_memorySize;            /* Velikost pameti pro GPIO */ 
  tick_gpio_out p_GPIO_IN_tick                 /* Pointer na funkci LOG_GPIO_Request() */
}GPIO_IN_InitTypeDef;


enum GPIO_IN_STATE{
	GPIO_IDLE = 0,
	GPIO_RUN,
	GPIO_ERR
};

/* GPIO_out_Exported_Functions */
void GPIO_IN_init(*GPIO_IN_InitTypeDef);  /* Inicializace pomoci deskriptoru */
GPIO_IN_STATE GPIO_IN_get_state();        /* Vrati stav prevodniku */
uint8_t GPIO_get_word();                  /* Precteni slova na vstupu */
uint16_t GPIO_get_pointer();              /* Vrati index do pole */
bool GPIO_is_buffer_overflowed();         /* Kontrola preteceni bufferu */
//void GPIO_LOG_IRQ_handler();              



#endif /*__<procesor>_GPIO_IN_H */

/************************ END OF FILE *****************************************/
