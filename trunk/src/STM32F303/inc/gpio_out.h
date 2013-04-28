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
#ifndef __procesor_GPIO_OUT_H
#define __procesor_GPIO_OUT_H

/* Includes ------------------------------------------------------------------*/

/* Signals & pinout ----------------------------------------------------------*/
#define DATA0_PORT	GPIOE
#define DATA0_PIN	GPIO_Pin_8	
#define DATA1_PORT	GPIOE
#define DATA1_PIN	GPIO_Pin_9
#define DATA2_PORT	GPIOE
#define DATA2_PIN	GPIO_Pin_10
#define DATA3_PORT	GPIOE
#define DATA3_PIN	GPIO_Pin_11
#define DATA4_PORT	GPIOE
#define DATA4_PIN	GPIO_Pin_12
#define DATA5_PORT	GPIOE
#define DATA5_PIN	GPIO_Pin_13
#define DATA6_PORT	GPIOE
#define DATA6_PIN	GPIO_Pin_14
#define DATA7_PORT	GPIOE
#define DATA7_PIN	GPIO_Pin_15

#define PORTS_CLOCKS RCC_AHBPeriph_GPIOC

/* GPIO_out_Exported_Functions */
void GPIO_OUT_init(void);                     /* Inicializace vystupnich pinu */
void GPIO_set_word(uint8_t _word);              /* Nastaveni pozadovaneho kodoveho slova */
uint8_t GPIO_get_setted_val(void);            /* Precte hodnotu, ktera byla skutecne nastavena */


#endif /*__<procesor>_GPIO_OUT_H */

/************************ END OF FILE *****************************************/
