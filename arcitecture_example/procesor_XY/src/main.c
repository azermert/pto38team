/*
 * main.c
 *
 *  Created on: Dec 15, 2011
 *      Author: Jakub Esenr
 */
/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "nmi_irq.h"
#include "main.h"
#include "uart.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

int main(void)
{
	UART_TypeDef uart1;
	
  uart1.UART_BaudRate=BAUDRATE_115200;  
  uart1.UART_StopBits=STOPBIT_1_5;
  uart1.UART_Parity=PARITY_NONE; 
	
	UART_init_struct(&uart1);
	
	while (1)
	{	

		
		
		
	}
}
