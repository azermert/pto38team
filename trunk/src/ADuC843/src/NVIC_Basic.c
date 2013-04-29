/****************************
	Soubor povoleni a ruseni preruseni a jejich priznaku

					*********************** */
#include "ADuC843.h"
#include "typedef.h"
#include "NVIC_Basic.h"

void NVIC_IntEnable (uint8_t Channel, uint8_t Priority){	//NVIC set anable bit & priority
/*	uint8_t	ISx;
	uint8_t	ISr;
	ISx = Channel/32;
	ISr = Channel - (32*ISx);
	NVIC->IP[Channel] = (Priority << 4);
	NVIC->ISER[ISx] |= (0x01 << ISr);*/
}

void NVIC_IntDisable (uint8_t Channel){				//NVIC clear enable bit
/*	uint8_t	ISx;
	uint8_t	ISr;
	ISx = Channel/32;
	ISr = Channel - (32*ISx);
	NVIC->ICER[ISx] |= (0x01 << ISr);*/
}

void NVIC_ClearPending (uint8_t Channel){			//NVIC clear pending bit
/*	uint8_t	ISx;
	uint8_t	ISr;
	ISx = Channel/32;
	ISr = Channel - (32*ISx);
	NVIC->ICPR[ISx] |= (0x01 << ISr);*/
}
