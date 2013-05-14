/****************************
	Soubor povoleni a ruseni preruseni a jejich priznaku

					*********************** */
#include "stm32f0xx.h"
#include "typedefs.h"
#include "NVIC_Basic.h"

void NVIC_IntEnable (u8 Channel, u8 Priority){	//NVIC set anable bit & priority
	u8	ISx;
	u8	ISr;
	ISx = Channel/32;
	ISr = Channel - (32*ISx);
	NVIC->IP[Channel] = (Priority << 4);
	NVIC->ISER[ISx] |= (0x01 << ISr);
}

void NVIC_IntDisable (u8 Channel){				//NVIC clear enable bit
	u8	ISx;
	u8	ISr;
	ISx = Channel/32;
	ISr = Channel - (32*ISx);
	NVIC->ICER[ISx] |= (0x01 << ISr);
}

void NVIC_ClearPending (u8 Channel){			//NVIC clear pending bit
	u8	ISx;
	u8	ISr;
	ISx = Channel/32;
	ISr = Channel - (32*ISx);
	NVIC->ICPR[ISx] |= (0x01 << ISr);
}
