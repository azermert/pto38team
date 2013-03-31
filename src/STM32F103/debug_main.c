

#include "stm32f10x.h"
#include "comm.h"
#include "uart.h"


void SystemInit(void);
void delay(uint32_t);

COMM_InitTypeDef commInit;	
BUFF_STATE xxx;
char chr = 0x61;
char znak;
uint8_t data[UART_BUFF_SIZE]="Ahoj tady pocitac";

int main (void){
	

	
	
	SystemInit();
	
	commInit.speed = COMM_SAFE_MODE;
	
	COMM_init(&commInit);	
	//data="Ahoj tady pocitac";
	
	while(1)
	{
		
		//COMM_send(data, 24);
		//xxx=COMM_print("hello world");
		delay(0x4fff);
		chr=COMM_get_bytes_available();
		xxx=COMM_get_in_buff_state();	
		if(xxx==BUFF_ALMOST_FULL){
			chr=COMM_get_bytes_available();
			znak=COMM_read(data, 60);
			delay(0x4);
		}
		/*
		xxx=COMM_put_char(chr);
		chr++;
		if(chr>0x78){
		chr=0x61;
		}
		if(xxx==BUFF_ALMOST_FULL){
			delay(0xf);
			UART_tick();
			while(xxx!=BUFF_DATA && xxx!=BUFF_FREE){
				xxx=COMM_get_out_buff_state();	
			}
			delay(0x1);

		}
		if(xxx==BUFF_FREE){
			delay(0xf);
		}
		delay(0xffff);*/		
	}
		
}



void SystemInit(void){
	//	CFGR = Clock configuration register	
 	// setting prescalers, PLL multipler, PLL source and source for MCO
	RCC->CFGR = RCC_CFGR_HPRE_DIV1			//		HPRE 		= AHB prescaler	
				|RCC_CFGR_PPRE1_DIV1		//		PRE1		= APB Low-speed prescaler (APB1)
				|RCC_CFGR_PPRE2_DIV1		//		PRE2		= APB high-speed prescaler (APB2)
				|RCC_CFGR_ADCPRE_DIV2		//		ADCPRE		= ADC prescaler
				|RCC_CFGR_PLLSRC			//		PLLSRC		= PLL entry clock source 			(0=HSI; 1=HSE)
				|RCC_CFGR_PLLXTPRE	  		//		PLLXTPRE	= LSB of division factor PREDIV1	(0=/1; 1=/2)
				|RCC_CFGR_PLLMULL_2
				;

	// turn on external oscilator
   	RCC->CR	|= RCC_CR_HSEON;

	// waiting for crystal to be ready
	while(RCC_CR_HSERDY !=	(RCC->CR & RCC_CR_HSERDY))
	{
		__nop();
	}

	// starting PLL
	RCC->CR	|= RCC_CR_PLLON;

	// waiting for PLL to be ready
	while(RCC_CR_PLLRDY !=	(RCC->CR & RCC_CR_PLLRDY))
	{
		__nop();
	}

	//switching system clock to PLL output
	RCC->CFGR |= RCC_CFGR_SW_PLL;

	// checking switch position
	while(RCC_CFGR_SWS_PLL !=	(RCC->CFGR & RCC_CFGR_SWS_PLL))
	{
		__nop();
	}
	delay(0x000FFF);
}

/*******************************************************************************
* Function Name  : delay
* Description    : Function for waiting
* Input          : nCount - vu32 (number of cycles)
* Output         : None
* Return         : None
*******************************************************************************/
void delay(uint32_t nCount) {
  for(; nCount != 0; nCount--);
}

