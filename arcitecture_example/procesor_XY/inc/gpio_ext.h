/**
  ******************************************************************************
  * @file    gpio_ext.h
  * @author  PTO Team
  * @version V1.0.0
  * @date    3/3/2013
  * @brief   This file contains all the functions prototypes for the GPIO_ext firmware 
  *          library.
  ******************************************************************************
*/ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __<procesor>_GPIO_EXT_H
#define __<procesor>_GPIO_EXT_H

/* Includes ------------------------------------------------------------------*/
/* Zadne includy zde nebudou!!!*/

typedef void (*tick_gpio_ext)(void);

typedef struct
{  
  tick_gpio_ext p_GPIO_EXT_tick;                 /* Pointer na funkci COUNT_GPIO_Request() */
}GPIO_EXT_InitTypeDef;


typedef enum
{
	GPIO_IDLE = 0,
	GPIO_RUN,
	GPIO_ERR
}GPIO_EXT_STATE;

/* GPIO_out_Exported_Functions */
void GPIO_EXT_init(struct GPIO_IN_InitTypeDef *p_GPIO_IN_desc); /* Inicializace pomoci deskriptoru */
void GPIO_EXT_start_counting();           /* Zacne pocitat pulzy pomoci preruseni */  
void GPIO_EXT_stop_counting();            /* Zastavi citani */
GPIO_EXT_STATE GPIO_EXT_get_state();      /* Vrati stav prevodniku */
//void GPIO_COUNT_IRQ_handler();              



#endif /*__<procesor>_GPIO_EXT_H */

/************************ END OF FILE *****************************************/
