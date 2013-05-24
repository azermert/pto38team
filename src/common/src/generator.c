/**
  ******************************************************************************
  * @file    generator.c
  * @author  PTO Team
  * @version V1.0.0
  * @date    24/3/2013
  * @brief   Knihovna zajistujici funkci generatoru. Obstarava vytvareni signalu a obsluhu DAC
  ******************************************************************************  
  */ 

/* Includes ------------------------------------------------------------------*/
//#include "<procesor>.h"
#include "typedefs.h"
#include "generator.h"
#include "dac.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/ 
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint16_t	lGenBuff[GEN_BUFF_LENGHT];
GEN_SIGNAL  lSigDefault	= {	GEN_SINE, 1000,	1500, 50, 1000 };
GEN_SIGNAL	gGenSignal = { GEN_SINE, 1000, 1500, 50, 1000 };


/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
 * @brief   Inicializace pomoci decsriptoru
 * @param  GEN desc
 * @retval None
 */ 
void GEN_init(GEN_InitTypeDef * p_GEN_desc)
{
	pto_DAC_InitTypeDef DAC_desc;
	
	DAC_desc.DAC_samplingFrequency = p_GEN_desc->GEN_sampleRate;
	DAC_desc.p_DAC_memory = (uint32_t *) lGenBuff;//p_GEN_desc->p_GEN_buffer->memory; z neznamych duvodu spolehlivejsi, bohuzel ztrata obecnosti
	DAC_desc.DAC_memorySize = p_GEN_desc->p_GEN_buffer->size;

	DAC_initialize(&DAC_desc);
}

/**
 * @brief  Do bufferu napocita vzorky pro dany signal, frekvenci offset atd
 * (pokud se jedna o arb signal, tak nedela nic)
 * @param  GEN_signal
 * @retval Pokud se povedlo nastavit tak true
 */ 
bool GEN_set_signal(GEN_SIGNAL* _sig)
{
	GEN_InitTypeDef		gen_desc;
	GEN_Buffer			genBuff;

	uint16_t	samples;
	uint16_t	duty;
	int16_t		i;

	gen_desc.p_GEN_buffer = &genBuff;
	genBuff.memory = &lGenBuff[0];
	
	//maximum moznych vzorku
	samples = DMA_RATE_MAX / _sig->frequency;
	if(samples > GEN_BUFF_LENGHT){
		samples = GEN_BUFF_LENGHT;
	}
	genBuff.size = samples;

	//zpetny dopocet SR (pro zpresneni)
	gen_desc.GEN_sampleRate = samples*_sig->frequency;

	//pocet vzorku pro prvni pulperiodu
	if(_sig->duty > 100){_sig->duty = 100;}

	duty = samples * _sig->duty;
	duty = duty / 100;

    for (i = 0; i < samples; i++) {
		int32_t	tmp;
	
		switch (_sig->GEN_signalType){
			case GEN_SQUARE:
				if(i<duty)
				{
					tmp = _sig->offset - _sig->amplitude;
				}
				else
				{
					tmp = _sig->offset + _sig->amplitude;
				}
				break;
			case GEN_TRIANGLE:

				if(i<duty)
				{
					tmp = _sig->amplitude * 2 *	i;
					tmp = tmp / duty;
					tmp = tmp + _sig->offset - _sig->amplitude;
				}
				else
				{
					tmp = - _sig->amplitude * 2 * (i-duty);
					tmp = tmp / (samples - duty);
					tmp = tmp + _sig->offset + _sig->amplitude;
				}
				break;
			case GEN_SINE:
				{
					float value = 0;
					float tmp_f;
					int8_t	sign = 0;
					int8_t	exp;
					uint16_t	fact;

					float phase = (float)i / samples;
					
					//prevedeme vsechny uhly do 1. kvadrantu
					if(phase > 0.75){
						phase = 1.0 - phase;
						sign = -1;	
					}
					else if(phase > 0.5)
					{
						phase = phase - 0.5;
						sign = -1;
					}
					else if(phase > 0.25)
					{
						phase = 0.5 - phase;
						sign = 1;
					}
					else
					{
						sign = 1;
					}
					
					phase = phase * 2.0 * 3.14159265358979323846;	//radians 
					tmp_f = phase;
					fact = 1;
					for (exp = 1; exp < 8; exp+=2)
					{
						value += (sign * tmp_f)/fact;

						//priprava na dalsi pruchod
						fact = fact * (exp+1) * (exp+2);
						sign = - sign;
						tmp_f = tmp_f * phase * phase;					
					}

					value = value * _sig->amplitude;
					value += _sig->offset;

					tmp = (int32_t)value;
				}
			break;
		
		}

		if(tmp < 0){
			lGenBuff[i] = 0;
		}   
		else if(tmp > DAC_MAX_VALUE)
		{
			lGenBuff[i] = DAC_MAX_VALUE;
		}
		else
		{
			lGenBuff[i] = tmp;
		}
	}

	GEN_init(&gen_desc);

	return TRUE;
}

/**
 * @brief  Odstartuje generovani signalu
 * @param  None
 * @retval None
 */ 
void GEN_start()													
{
	GEN_set_signal(&gGenSignal);
	DAC_DMA_start();
}

/**
 * @brief  Zastavi generovani signalu 
 * @param  None
 * @retval None
 */ 
void GEN_stop()
{
	DAC_DMA_stop();
}

/**
 * @brief  Vrati stav generatoru
 * @param  Gen state
 * @retval None
 */ 
GEN_STATE GEN_get_state()
{
	switch (DAC_get_state())
	{
		case DAC_IDLE:
			return GEN_IDLE;
		case DAC_RUN:
		case DAC_DMA_RUN:
			return GEN_RUN;
		case DAC_ERR:
		default:
			return GEN_ERR;
	};
}



/************************ END OF FILE *****************************************/
