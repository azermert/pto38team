
#include "uart.h"

#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_gpio.h"
#include "..\NVIC_Basic.h"

/* Private variables*/
UART_DESC*	uartRegister[] = {NULL,NULL,NULL};		//!<Ukazatele do prerusovacich rutin

/* Routines */
void urtSendTick(UART_DESC * _urt){
	if(_urt->parent.isSend){
		if( USART_GetFlagStatus(_urt->usartBase, USART_FLAG_TXE) == SET) {
			_urt->usartBase->DR = _urt->parent.getChar(&_urt->parent);
		}
	}
}

void urtRcvRq(UART_DESC * _urt){
	if(	USART_GetFlagStatus(_urt->usartBase, USART_FLAG_RXNE) == SET){
		_urt->parent.putChar(&_urt->parent, _urt->usartBase->DR); 		
	}	
}

void USART2_IRQHandler(void){
	UART_DESC * urt = uartRegister[2];

	urtRcvRq(urt);

	USART_ClearITPendingBit(urt->usartBase, USART_IT_RXNE);
	NVIC_ClearPending(USART2_IRQn);	
}

void UART_Tick(struct COMM_DESC * _desc){
	UART_DESC * urt = (UART_DESC*)_desc;

	urtSendTick(urt);	//send tick
	urtRcvRq(urt);//receive tick




}

void initHW_UART(UART_DESC * _urt){
	GPIO_InitTypeDef	GPIO_InitStructure;

	switch(_urt->usart_No){
		case 2:
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
			
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
			GPIO_Init(GPIOA, &GPIO_InitStructure);
		
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
			GPIO_Init(GPIOA, &GPIO_InitStructure);
			break;
		default:break;	
	}
}

bool init_UART(UART_DESC * _urt, u8 _usartNumber){

	USART_InitTypeDef USART_InitStruct;
	_urt->usart_No = _usartNumber;
	
	USART_StructInit(&USART_InitStruct);	 	//8bit, parity_none, stopbit_1 ..

	switch(_urt->usart_No){
		case 2:
			USART_InitStruct.USART_BaudRate = 115200;
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
			_urt->usartBase = (USART_TypeDef*)USART2_BASE;
			uartRegister[2] = _urt;
			break;
		default:
			return FALSE;	
	}

	USART_DeInit(_urt->usartBase);
	USART_Init(_urt->usartBase, &USART_InitStruct);

	init_Comm(&_urt->parent);

	_urt->Tick = _urt->parent.Tick;
	_urt->parent.Tick = &UART_Tick;

	USART_ITConfig(_urt->usartBase, USART_IT_RXNE, ENABLE);
	NVIC_IntEnable(USART2_IRQn,0x02);

	initHW_UART(_urt);
	USART_Cmd(_urt->usartBase, ENABLE);

	return TRUE;
}
