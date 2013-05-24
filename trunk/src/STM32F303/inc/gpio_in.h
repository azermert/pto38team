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
#ifndef __procesor_GPIO_IN_H
#define __procesor_GPIO_IN_H

#define PORTS_CLOCKS RCC_AHBPeriph_GPIOD

#define DATA0_PORT	GPIOD
#define DATA0_PIN	GPIO_Pin_8	
#define DATA1_PORT	GPIOD
#define DATA1_PIN	GPIO_Pin_9
#define DATA2_PORT	GPIOD
#define DATA2_PIN	GPIO_Pin_10
#define DATA3_PORT	GPIOD
#define DATA3_PIN	GPIO_Pin_11
#define DATA4_PORT	GPIOD
#define DATA4_PIN	GPIO_Pin_12
#define DATA5_PORT	GPIOD
#define DATA5_PIN	GPIO_Pin_13
#define DATA6_PORT	GPIOD
#define DATA6_PIN	GPIO_Pin_14
#define DATA7_PORT	GPIOD
#define DATA7_PIN	GPIO_Pin_15


/* Includes ------------------------------------------------------------------*/
/* Zadne includy zde nebudou!!!*/

typedef void (*tick_gpio_out)(void);

typedef struct
{
  uint32_t GPIO_IN_samplingFrequency;        
  uint32_t * p_GPIO_IN_memory;            			/* Pointer na pamet kam muze GPIO zapisovat */ 
  uint32_t GPIO_IN_memorySize;            			/* Velikost pameti pro GPIO */ 
  tick_gpio_out p_GPIO_IN_tick;                 /* Pointer na funkci LOG_GPIO_Request() */
}GPIO_IN_InitTypeDef;


typedef enum
{
	GPIO_IDLE = 0,
	GPIO_RUN,
	GPIO_ERR
}GPIO_IN_STATE;

/* GPIO_in_Exported_Functions */
void GPIO_IN_init(GPIO_IN_InitTypeDef * _desc);  /* Inicializace pomoci deskriptoru */
GPIO_IN_STATE GPIO_IN_get_state(void);        /* Vrati stav prevodniku */
uint8_t GPIO_get_word(void);                  /* Precteni slova na vstupu */
uint16_t GPIO_get_pointer(void);              /* Vrati index do pole */
bool GPIO_is_buffer_overflowed(void);         /* Kontrola preteceni bufferu */
//void GPIO_LOG_IRQ_handler();              



#endif /*__<procesor>_GPIO_IN_H */

/************************ END OF FILE *****************************************/
