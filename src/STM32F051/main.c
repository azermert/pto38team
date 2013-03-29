#include "stm32f0xx.h"
#include "stm32f0xx_conf.h"
#include "stm32f0xx_gpio.c"
#include "stm32f0xx_rcc.c"
#include "gpio_out.h"

uint8_t pom = 0;
void SystemInit(){
	GPIO_OUT_init(); // inicializace vystupu
	}

int main(){

	GPIO_set_word(0xff); // nastaveni vystupu
 	 
 	pom = GPIO_get_setted_val();
	
// 	while (1)
// 	{
//    }
}

