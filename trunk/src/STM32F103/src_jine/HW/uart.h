#ifndef _UART_H_
#define _UART_H_

#include "misc.h"
#include "stm32f10x_usart.h"
#include "..\typeDef.h"
#include "..\comm.h"

/* User defined values */
typedef u32 URT_BAUDRATE;
typedef u16 URT_PARITY;

typedef struct{
	struct COMM_DESC 		parent;
	u8				usart_No;
	USART_TypeDef * usartBase;

	cmTICK			Tick;

	URT_BAUDRATE	baudrate;


} UART_DESC;


/* Uart defifnitions*/




/* Exported function prototypes */

bool init_UART(UART_DESC * _desc, u8 _usartNumber);

	
#endif
