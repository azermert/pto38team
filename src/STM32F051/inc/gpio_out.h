/**
  ******************************************************************************
  * @file    gpio_out.h
  * @author  PTO Team
  * @version V1.0.0
  * @date    3/3/2013
  * @brief   This file contains all the functions prototypes for the GPIO_out firmware 
  *          library.
  ******************************************************************************
*/ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __stm32f0xx_GPIO_OUT_H
#define __stm32f0xx_GPIO_OUT_H

/* Includes ------------------------------------------------------------------*/
/* Zadne includy zde nebudou!!!*/

/* GPIO_out_Exported_Functions */
void GPIO_OUT_init();                     /* Inicializace vystupnich pinu */
void GPIO_set_word(uint8_t);              /* Nastaveni pozadovaneho kodoveho slova */
uint8_t GPIO_get_setted_val();            /* Precte hodnotu, ktera byla skutecne nastavena */


#endif /*__stm32f0xx_GPIO_OUT_H */

/************************ END OF FILE *****************************************/
