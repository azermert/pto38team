/**
  ******************************************************************************
  * @file    gpio_out.c
  * @author  PTO Team
  * @version V1.0.0
  * @date    3/3/2013
  * @brief   Tato knihovna obsluhuje vystupni GPIO piny           
  *       
  ******************************************************************************  
  */ 

/* Includes ------------------------------------------------------------------*/
#include "ADuC843.h"
#include "typedef.h"
#include "gpio_out.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/ 
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint8_t lStoredVal;
bool lRunning = FALSE;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Vrati hodnotu, ktera byla skutecne nastavena
  * @param  None
  * @retval uint8_t - nastavena hodnota
  */
uint8_t GPIO_get_setted_val()
{
  return lStoredVal;
}                       

/**
  * @brief  Nastavi na vystup dane kodove slovo
  * @param  uint8_t - kodove slovo
  * @retval None
  */
void GPIO_set_word(uint8_t _word)
{/*
	if(lRunning == FALSE){
		return;
	}

  lStoredVal = _word;
//D0
	if(_word & 0x01){
		GPIO_SetBits(DATA0_PORT, DATA0_PIN);
	}else{
		GPIO_ResetBits(DATA0_PORT, DATA0_PIN);
	}
//D1
	if(_word & 0x02){
		GPIO_SetBits(DATA1_PORT, DATA1_PIN);
	}else{
		GPIO_ResetBits(DATA1_PORT, DATA1_PIN);
	}
//D2
	if(_word & 0x04){
		GPIO_SetBits(DATA2_PORT, DATA2_PIN);
	}else{
		GPIO_ResetBits(DATA2_PORT, DATA2_PIN);
	}
//D3
	if(_word & 0x08){
		GPIO_SetBits(DATA3_PORT, DATA3_PIN);
	}else{
		GPIO_ResetBits(DATA3_PORT, DATA3_PIN);
	}
//D4
	if(_word & 0x10){
		GPIO_SetBits(DATA4_PORT, DATA4_PIN);
	}else{
		GPIO_ResetBits(DATA4_PORT, DATA4_PIN);
	}
//D5
	if(_word & 0x20){
		GPIO_SetBits(DATA5_PORT, DATA5_PIN);
	}else{
		GPIO_ResetBits(DATA5_PORT, DATA5_PIN);
	}
//D6
	if(_word & 0x40){
		GPIO_SetBits(DATA6_PORT, DATA6_PIN);
	}else{
		GPIO_ResetBits(DATA6_PORT, DATA6_PIN);
	}
//D7
	if(_word & 0x80){
		GPIO_SetBits(DATA7_PORT, DATA7_PIN);
	}else{
		GPIO_ResetBits(DATA7_PORT, DATA7_PIN);
	}*/
}                

/**
  * @brief  Inicializace
  * @param  None
  * @retval None
  */
void GPIO_OUT_init(void)
{/*
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(PORTS_CLOCKS, ENABLE);

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

	lRunning = TRUE;  	//priznak inicializace
	GPIO_set_word(0);*/
}
/************************ END OF FILE *****************************************/
