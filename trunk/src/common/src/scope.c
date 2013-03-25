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

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/ 
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
 * @brief  Inicializace pomoci decsriptoru
 * @param  desc
 * @retval None
 */ 
void SCOPE_init(SCOPE_InitTypeDef * p_SCOPE_desc)
{
	//TODO
}; 

/**
 * @brief  Nastaveni rychlosi vzorkovani*
 * @param  sample rate
 * @retval None
 */ 
void SCOPE_set_sample_rate(uint32_t smpRate)
{
	//TODO
};

/**
 * @brief  Hodnota pro voltmeter vcetne filtrace (samples = pocet vzorku pro prumerovani) 
 * @param  samples
 * @retval voltage
 */ 
uint16_t SCOPE_get_voltage (uint8_t samples)
{
	//TODO (udelat par odberu a mozna zahrnout i prumerovani)
	return 0;
};

/**
 * @brief  odstartovani mereni
 * @param  None
 * @retval None
 */ 
void SCOPE_start_meas(void)
{
	//TODO
};

/**
 * @brief  zastaveni mereni
 * @param  None
 * @retval None
 */ 
void SCOPE_stop_meas(void)
{
	//TODO
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
	//TODO
};

/**
 * @brief  nastaveni trigger modu
 * @param  trigger mode
 * @retval None
 */ 
void SCOPE_set_trigger_mode(SCOPE_TRIGGER_MODE trigMode)
{
	//TODO
};

/**
 * @brief  nastaveni hrany pro trigger
 * @param  trigger edge
 * @retval None
 */ 
void SCOPE_set_trigger_edge(SCOPE_TRIGGER_EDGE trigEdge)
{
	//TODO
};

/**
 * @brief  Vrati stav knihovny osciloskopu
 * @param  None
 * @retval SCOPE state
 */ 
SCOPE_STATE SCOPE_get_state(void)
{
	//TODO
	return SCOPE_ERR;
};

/**
 * @brief  Kontrola triggeru a konce mereni pri kazdem odmerenem vzorku (volano z ADC)
 * @param  None
 * @retval None
 */ 
void SCOPE_ADC_request(void)
{
	//TODO
};

/************************ END OF FILE *****************************************/

