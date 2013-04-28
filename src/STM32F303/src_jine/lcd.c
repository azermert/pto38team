
#include "lcd.h"
#include "timeBase.h"

#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"


/* Definition of constants */
#define lcd_ini0 0x02	//4 bit interface (for init)
#define lcd_ini1 0x28	//function set
#define lcd_home 0x02	//return to position 0
#define lcd_clr  0x01	//clear display

/* Local variables */

/* Function prototypes */
void LcdSendNibble(u8 data,u8 rs);
void Lcd_HW_init(void);
#ifdef DATA_INTERFACE_8Bits
	void LcdSendByte(u8 data,u8 rs);
#endif

/* Routines */
void init_LCD(void){
	/* LCD Initialization				*/
	Lcd_HW_init();		//Pins initialization, RCC enable
	//Lcd function init
	#ifndef DATA_INTERFACE_8Bits
		LcdSendNibble(lcd_ini0,inst);
	#endif
	LcdStrChar(lcd_ini1,inst);
	LcdStrChar(lcd_entry,inst);
	LcdStrChar(lcd_visible,inst);
	LcdStrChar(lcd_home,inst);
	delay(2000);	//2 ms


}

void Lcd_HW_init(void){

	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(LCD_PORTS_CLOCKS, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = RS_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(RS_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = E_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(E_PORT, &GPIO_InitStructure);


	#ifdef DATA_INTERFACE_8Bits
		GPIO_InitStructure.GPIO_Pin = DATA0_PIN;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
		GPIO_Init(DATA0_PORT, &GPIO_InitStructure);
		
		GPIO_InitStructure.GPIO_Pin = DATA1_PIN;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
		GPIO_Init(DATA1_PORT, &GPIO_InitStructure);
		
		GPIO_InitStructure.GPIO_Pin = DATA2_PIN;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
		GPIO_Init(DATA2_PORT, &GPIO_InitStructure);
		
		GPIO_InitStructure.GPIO_Pin = DATA3_PIN;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
		GPIO_Init(DATA3_PORT, &GPIO_InitStructure);
	#endif
	
	GPIO_InitStructure.GPIO_Pin = DATA4_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(DATA4_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = DATA5_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(DATA5_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = DATA6_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(DATA6_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = DATA7_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(DATA7_PORT, &GPIO_InitStructure);

}

void LcdHome(void){
	LcdStrChar(lcd_home,inst);
	delay(5000);	//5 ms	
}

void LcdClear(void){
	LcdStrChar(lcd_clr,inst);
	delay(5000);	//5 ms	
}

void LcdStrText(char *text) {
	while (*text)  {
    	LcdStrChar(*text++,dat);
  	}
}

void LcdStrChar(u8 data,u8 rs) {
	#ifndef DATA_INTERFACE_8Bits
		LcdSendNibble(data & 0x0F0, rs);
		LcdSendNibble((data<<4)&0x0F0, rs);
	#else
	 	LcdSendByte(data,rs);
	#endif
}


void LcdSendNibble(u8 data,u8 rs) {
	if(rs){
		GPIO_SetBits(RS_PORT, RS_PIN);
	}else{
		GPIO_ResetBits(RS_PORT, RS_PIN);
	}
	GPIO_SetBits(E_PORT, E_PIN);//E set
	
	//adjust output data   
	//void GPIO_SetBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
	//void GPIO_ResetBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);

	//D4
		if(data & 0x10){
			GPIO_SetBits(DATA4_PORT, DATA4_PIN);
		}else{
			GPIO_ResetBits(DATA4_PORT, DATA4_PIN);
		}
	//D5
		if(data & 0x20){
			GPIO_SetBits(DATA5_PORT, DATA5_PIN);
		}else{
			GPIO_ResetBits(DATA5_PORT, DATA5_PIN);
		}
	//D6
		if(data & 0x40){
			GPIO_SetBits(DATA6_PORT, DATA6_PIN);
		}else{
			GPIO_ResetBits(DATA6_PORT, DATA6_PIN);
		}
	//D7
		if(data & 0x80){
			GPIO_SetBits(DATA7_PORT, DATA7_PIN);
		}else{
			GPIO_ResetBits(DATA7_PORT, DATA7_PIN);
		}

	//write data
	delay(10);
	E_PORT->BRR |= E_PIN;		//E reset ~ write
	delay(40);		//40 us
	
}

#ifdef DATA_INTERFACE_8Bits
	void LcdSendByte(u8 data,u8 rs) {
		if(rs){
			GPIO_SetBits(RS_PORT, RS_PIN);
		}else{
			GPIO_ResetBits(RS_PORT, RS_PIN);
		}
		GPIO_SetBits(E_PORT, E_PIN);//E set
	
	
		//D0
			if(data & 0x01){
				GPIO_SetBits(DATA0_PORT, DATA0_PIN);
			}else{
				GPIO_ResetBits(DATA0_PORT, DATA0_PIN);
			}
		//D1
			if(data & 0x02){
				GPIO_SetBits(DATA1_PORT, DATA1_PIN);
			}else{
				GPIO_ResetBits(DATA1_PORT, DATA1_PIN);
			}
		//D2
			if(data & 0x04){
				GPIO_SetBits(DATA2_PORT, DATA2_PIN);
			}else{
				GPIO_ResetBits(DATA2_PORT, DATA2_PIN);
			}
		//D3
			if(data & 0x08){
				GPIO_SetBits(DATA3_PORT, DATA3_PIN);
			}else{
				GPIO_ResetBits(DATA3_PORT, DATA3_PIN);
			}
		//D4
			if(data & 0x10){
				GPIO_SetBits(DATA4_PORT, DATA4_PIN);
			}else{
				GPIO_ResetBits(DATA4_PORT, DATA4_PIN);
			}
		//D5
			if(data & 0x20){
				GPIO_SetBits(DATA5_PORT, DATA5_PIN);
			}else{
				GPIO_ResetBits(DATA5_PORT, DATA5_PIN);
			}
		//D6
			if(data & 0x40){
				GPIO_SetBits(DATA6_PORT, DATA6_PIN);
			}else{
				GPIO_ResetBits(DATA6_PORT, DATA6_PIN);
			}
		//D7
			if(data & 0x80){
				GPIO_SetBits(DATA7_PORT, DATA7_PIN);
			}else{
				GPIO_ResetBits(DATA7_PORT, DATA7_PIN);
			}
	
		//write data
		delay(10);
		E_PORT->BRR |= E_PIN;		//E reset ~ write
		delay(40);		//40 us
		
	}
#endif
