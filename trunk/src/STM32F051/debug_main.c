

#include "stm32f10x.h"
#include "comm.h"


void SystemInit(void){};
	
int main (void){
	
	COMM_InitTypeDef commInit;
	commInit.speed = COMM_SAFE_MODE;
	
	COMM_init(&commInit);




}
