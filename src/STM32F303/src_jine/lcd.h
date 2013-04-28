#ifndef _LCD_H
#define _LCD_H

#include "misc.h"

	/* List of commands */
	#define lcd_entry	0x06	//entry mode
	#define lcd_visible	0x0E	//display on
	#define lcd_line1	0x80	//DDRAM adress base
	#define lcd_line2	0xC0	//DDRAM adress of 2nd line
	#define inst	0
	#define dat		!inst
	
	/* Pinout description, interface select */
	//#define DATA_INTERFACE_8Bits		//uncomment if full data interface is used

	#define RS_PORT		GPIOC
	#define RS_PIN		GPIO_Pin_12
	#define E_PORT		GPIOC
	#define E_PIN		GPIO_Pin_11

	#ifdef DATA_INTERFACE_8Bits
		#define DATA0_PORT	GPIOx		//lowest pin
		#define DATA0_PIN	GPIO_Pin_x	
		#define DATA1_PORT	GPIOx
		#define DATA1_PIN	GPIO_Pin_x
		#define DATA2_PORT	GPIOx
		#define DATA2_PIN	GPIO_Pin_x
		#define DATA3_PORT	GPIOx
		#define DATA3_PIN	GPIO_Pin_x
	#endif

	#define DATA4_PORT	GPIOC		//lowest pin of 4 bit data interface
	#define DATA4_PIN	GPIO_Pin_0	
	#define DATA5_PORT	GPIOC
	#define DATA5_PIN	GPIO_Pin_1
	#define DATA6_PORT	GPIOC
	#define DATA6_PIN	GPIO_Pin_2
	#define DATA7_PORT	GPIOC
	#define DATA7_PIN	GPIO_Pin_3
	
	#define LCD_PORTS_CLOCKS RCC_APB2Periph_GPIOC	//list of all used ports  i.e. RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB
	
	/* Accesible function prototypes */
	void init_LCD(void);
	
	void LcdStrText(char *text);
	void LcdStrChar(u8 data,u8 rs);
	
	void LcdHome(void);
	void LcdClear(void);

#endif
