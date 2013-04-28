#ifndef _SINGLEPINPERIPH_H
#define _SINGLEPINPERIPH_H

#include "misc.h"

/* Usage definitions*/
	#define IRC_INDEX		//uncomment if index signal is used
	#define POSITION_MARK	//uncomment if position mark is used
//	#define CURRENT_FB		//uncomment if current measurement is used


/* Peripherial dependent definitions*/
#ifdef IRC_INDEX
	/* Pinout & options */
	#define IRC_INDEX_PORT	GPIOA
	#define IRC_INDEX_PIN	GPIO_Pin_12
	
	#define IRC_INDEX_IRQn			EXTI15_10_IRQn
	#define IRC_INDEX_Handler		EXTI15_10_IRQHandler
	#define IRC_EXTI_PinSource		GPIO_PinSource12
	#define IRC_EXTI_PortSource		GPIO_PortSourceGPIOA
	#define IRC_INDEX_GPIO_CLOCK 	RCC_APB2Periph_GPIOA

	/* Constants definitions */
	#define INDEX_InActive	0
	#define INDEX_Active	!INDEX_InActive

	/* Function prototypes */
	void init_Index(void);
	u8 getIndexState(void);
	void waitToIndexInActive(void);
	void waitToIndexActive(void);
	void indexIntEnable(void);
	void indexIntDisable(void);	
#endif//IRC_INDEX

#ifdef POSITION_MARK
	/* Pinout & options */
	#define POS_MARK_PORT	GPIOC
	#define POS_MARK_PIN	GPIO_Pin_10
	#define POS_MARK_GPIO_CLOCK 	RCC_APB2Periph_GPIOC

	/* Constants definitions */
	#define POS_MARK_Reset	0
	#define POS_MARK_Set	!POS_MARK_Reset

	/* Function prototypes */
	void init_Mark(void);
	u8 getMarkState(void);
	void waitToMarkSet(void);
	void waitToMarkReset(void);
#endif//POSITION_MARK

#ifdef CURRENT_FB
	/* Pinout & options */
	#define CURRENT_IN_PORT			GPIOC
	#define CURRENT_IN_PIN			GPIO_Pin_5
	#define CURRENT_ADC_CHANNEL		ADC_Channel_15
	#define CURRENT_IN_CLOCKS 		RCC_APB2Periph_GPIOC|RCC_APB2Periph_ADC1

	/* Interrupt - analog watchdog */
	#define CURRENT_ADC				ADC1
	#define CURRENT_IRQn			ADC1_2_IRQn
	#define CURRENT_Handler			ADC_IRQHandler

	/* Constants definitions */
	#define CURRENT_UPPER_LIMIT_mA	4000
	#define CURRENT_RESOLUTION_mA	2.51

	#define CURRENT_HTR_ADD		CURRENT_UPPER_LIMIT_mA/CURRENT_RESOLUTION_mA
	#define CURRENT_InLimit		0
	#define CURRENT_OverLoad	!CURRENT_InLimit

	/* Function prototypes */
	void init_CurrentFB(void);
	float Current_mA(void);
	u8	Current_getFlag(void);
	void Current_clearFlag(void);

#endif//CURRENT_FB

/* Exported common functions prototypes */
void init_SinglePinPeriph(void);

#endif//_SINGLEPINPERIPH_H
