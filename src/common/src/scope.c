/**
  ******************************************************************************
  * @file    scope.c
  * @author  PTO Team
  * @version V1.0.0
  * @date    24/3/2013
  * @brief   Knihovna obstaravyjici vsechny funkce osciloskopu a voltmetru.
	*						Obstarava i vsechnu komunikaci s ADC (inicializaci a obsluhu adc requestu)
  ******************************************************************************  
  */ 

/* Includes ------------------------------------------------------------------*/
//#include "<procesor>.h"
#include "stm32f10x.h"
#include "scope.h"

#include "adc.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/ 
/* Private macro -------------------------------------------------------------*/
/* Variables ---------------------------------------------------------*/
SCOPE_TypeDef gSCOPE;
SCOPE_Buffer lSCOPE_buff;

#define DATA_SIZE 2048
uint16_t lData[DATA_SIZE];

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

void SCOPE_ADC_request(uint16_t _value);

/**
 * @brief  Nastaveni rychlosi vzorkovani*
 * @param  sample rate
 * @retval None
 */ 
void SCOPE_set_sample_rate(uint32_t smpRate)
{
	PTO_ADC_InitTypeDef ADC_i;

	gSCOPE.SCOPE_samplingFrequency = smpRate;
	   
	ADC_i.ADC_samplingFrequency = smpRate;
	ADC_i.p_ADC_tick = &SCOPE_ADC_request; 
	ADC_init(&ADC_i);
};

/**
 * @brief  Hodnota pro voltmeter vcetne filtrace (samples = pocet vzorku pro prumerovani) 
 * @param  samples
 * @retval voltage
 */ 
uint16_t SCOPE_get_voltage (uint8_t samples)
{
	return ADC_meas_sample();
};

/**
 * @brief  odstartovani mereni
 * @param  None
 * @retval None
 */ 
void SCOPE_start_meas(void)
{
	ADC_circle_meas_start();
};


void SCOPE_setPreTrigger(float _value){
	gSCOPE.p_SCOPE_buffer->preTrigger = (_value * DATA_SIZE);	
}

/**
 * @brief  zastaveni mereni
 * @param  None
 * @retval None
 */ 
void SCOPE_stop_meas(void)
{
	ADC_circle_meas_stop();
};

/**
 * @brief Single mereni nejvyssi rychlosti 
 * @param  None
 * @retval None
 */ 
void SCOPE_DMA_meas(void)
{
	//TODO
};

/**
 * @brief  nastaveni urovne pro trigger
 * @param  trigger leve
 * @retval None
 */ 
void SCOPE_set_trigger_level(uint16_t trigLevel)
{
	gSCOPE.SCOPE_triggerLevel = trigLevel;
};

/**
 * @brief  nastaveni trigger modu
 * @param  trigger mode
 * @retval None
 */ 
void SCOPE_set_trigger_mode(SCOPE_TRIGGER_MODE trigMode)
{
	gSCOPE.SCOPE_triggerMode = trigMode;
};

/**
 * @brief  nastaveni hrany pro trigger
 * @param  trigger edge
 * @retval None
 */ 
void SCOPE_set_trigger_edge(SCOPE_TRIGGER_EDGE trigEdge)
{
	gSCOPE.SCOPE_triggerEdge = trigEdge;
};

/**
 * @brief  Vrati stav knihovny osciloskopu
 * @param  None
 * @retval SCOPE state
 */ 
SCOPE_STATE SCOPE_get_state(void)
{
	return gSCOPE.SCOPE_state;
};


SCOPE_BUFF_STATE storeVal(SCOPE_Buffer * _buff, uint16_t _val){



	return 	SCOPE_BUFF_FULL;
}



uint16_t set_trig(SCOPE_Buffer * _buff){

	return 	0;
}

/**
 * @brief  Kontrola triggeru a konce mereni pri kazdem odmerenem vzorku (volano z ADC)
 * @param  None
 * @retval None
 */ 
void SCOPE_ADC_request(uint16_t _value)
{
	//get value from adc , save value to buffer
	static uint16_t dataRemain;
	bool triggerEvent = FALSE;
	static bool trigEnable = FALSE;

	switch (gSCOPE.SCOPE_state){
	   	case SCOPE_TRIGGER_WAIT:			//getting data before trigger

			switch (gSCOPE.SCOPE_triggerEdge){
				case SCOPE_RISING:
					if(_value > gSCOPE.SCOPE_triggerLevel){
						if(trigEnable){
							triggerEvent = TRUE;
						}
					}
					if(_value < gSCOPE.SCOPE_triggerLevel){
						trigEnable = TRUE;
					}
					break;
				case SCOPE_FALLING:
					if(_value < gSCOPE.SCOPE_triggerLevel){
						if(trigEnable){
							triggerEvent = TRUE;
						}
					}
					if(_value > gSCOPE.SCOPE_triggerLevel){
						trigEnable = TRUE;
					}
					break;
			}

			if( triggerEvent ){
				gSCOPE.SCOPE_state = SCOPE_SAMPLING;
				trigEnable = FALSE;
				set_trig(gSCOPE.p_SCOPE_buffer);
				dataRemain = gSCOPE.p_SCOPE_buffer->size - gSCOPE.p_SCOPE_buffer->preTrigger;
			}
			if( storeVal(gSCOPE.p_SCOPE_buffer,_value) == SCOPE_BUFF_FULL){
				gSCOPE.SCOPE_state = SCOPE_ERR;
			}	
			dataRemain--;	   	

			break;
		case SCOPE_SAMPLING:		//getting data after trigger

			if( storeVal(gSCOPE.p_SCOPE_buffer,_value) == SCOPE_BUFF_FULL){
				gSCOPE.SCOPE_state = SCOPE_ERR;
			}	
			dataRemain--;
			if(dataRemain == 0){
				gSCOPE.SCOPE_state = SCOPE_DONE;
			}
			break;
			
		case SCOPE_IDLE:				//NOP
		case SCOPE_DONE:
		case SCOPE_ERR:
		default:
			break;
	}//switch




};

/**
 * @brief  Inicializace pomoci decsriptoru
 * @param  desc
 * @retval None
 */ 
void SCOPE_buf_init(SCOPE_Buffer * _buff){
	_buff->memory = lData;
	_buff->size = DATA_SIZE;
	_buff->writeIndex = 0;
	_buff->readIndex = 0;
	_buff->state = SCOPE_BUFF_FREE;
}


void SCOPE_init(SCOPE_TypeDef * _desc)
{
	PTO_ADC_InitTypeDef ADC_i;
   
	ADC_i.ADC_samplingFrequency = _desc->SCOPE_samplingFrequency;
	ADC_i.p_ADC_tick = &SCOPE_ADC_request; 
	ADC_init(&ADC_i);
	
	_desc->p_SCOPE_buffer = &lSCOPE_buff;
	_desc->SCOPE_state = SCOPE_IDLE;
	
	SCOPE_buf_init(_desc->p_SCOPE_buffer);		
};

/************************ END OF FILE *****************************************/

