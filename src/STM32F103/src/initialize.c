
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"

#include "initialize.h"
#include "timeBase.h"
#include "systemClock.h"
#include "uart.h"
#include "adc.h"   
#include "generator.h"
#include "scope.h"
//#include "dac.h"

//#define DAC_MEM_SIZE 20
//uint16_t lDAC_memory[DAC_MEM_SIZE];	 



void init_Discovery(void){

	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOA, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void serialCommInit(void){
	COMM_InitTypeDef commInit;

	commInit.speed = COMM_NORMAL;	
	COMM_init(&commInit);
}

void initialize(void){

	init_SystemClock();	  	//RCC config

	init_Discovery();	  	//leds & user button

	init_TimeBase();	  	//timebase & clock

	serialCommInit();		//uart2 init
	
	/* pozn. kdyz byl generator az za osciloskopem,
	z neznameho duvodu to obcas slo, obcas(casteji) neslo.
	(pri krokovani vse OK, v behu nikoliv)	*/

	GEN_set_signal(&gGenSignal);

	delay(1000000);		  	//pro jistotu
		
	
	gSCOPE.SCOPE_samplingFrequency = 70000;      
	gSCOPE.SCOPE_triggerEdge = SCOPE_RISING;
	gSCOPE.SCOPE_triggerMode = TRIG_AUTO;
	gSCOPE.SCOPE_triggerLevel = 2304;
	

	SCOPE_init(&gSCOPE);
	
	SCOPE_setPreTrigger(100);
	


	

/*	DAC_desc.DAC_samplingFrequency = 20000;
	DAC_desc.p_DAC_memory =  lDAC_memory;
	DAC_desc.DAC_memorySize = DAC_MEM_SIZE;
	DAC_init(&DAC_desc);   	  */

/*	for(i = 0; i < DAC_MEM_SIZE;i++)
	{
		gDAC_memory[i] = (0x0FFF * i)/(DAC_MEM_SIZE-1);
	}				*/
/*	lDAC_memory[0] = 2048;
	lDAC_memory[1] = 2680;
	lDAC_memory[2] = 3250;
	lDAC_memory[3] = 3703;
	lDAC_memory[4] = 3995;
	lDAC_memory[5] = 4095;
	lDAC_memory[6] = 3995;
	lDAC_memory[7] = 3703;
	lDAC_memory[8] = 3250;
	lDAC_memory[9] = 2680;
	lDAC_memory[10] = 2048;
	lDAC_memory[11] = 1415;
	lDAC_memory[12] = 844;
	lDAC_memory[13] = 391;
	lDAC_memory[14] = 100;
	lDAC_memory[15] = 0;
	lDAC_memory[16] = 100;
	lDAC_memory[17] = 391;
	lDAC_memory[18] = 844;
	lDAC_memory[19] = 1415;		*/  



}
