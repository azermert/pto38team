/**
  ******************************************************************************
  * @file    log_analyser.c
  * @author  PTO Team
  * @version V1.0.0
  * @date    24/3/2013
  * @brief   Knihovna zajistujici funkci logickeho analyzatoru. 
	*						Buffer obsahuje dve pole pro RLE kodovani
  ******************************************************************************  
  */ 

/* Includes ------------------------------------------------------------------*/
//#include "<procesor>.h"
#include "typedefs.h"
#include "log_analyser.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/ 
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


/**
  * @brief  inicializace pomoci descriptoru
  * @param  None
  * @retval None
  */
void LA_init(LA_InitTypeDef * p_LA_desc);

/**
  * @brief  nastavi rychlost vzorkovani
  * @param  None
  * @retval None
  */
void LA_set_sample_rate(uint32_t smpRate); 	

/**
  * @brief  odstartovani mereni
  * @param  None
  * @retval None
  */
void LA_start_meas(void);

/**
  * @brief  zastaveni mereni
  * @param  None
  * @retval None
  */
void LA_stop_meas(void);

/**
  * @brief  nastavi trigger na urcite kanaly a funki AND/OR  
  * @param  trigger logic, trigger channels
  * @retval None
  */
void LA_set_trigger_channel(LA_TRIGGER_LOGIC trigLog, uint8_t trigChan);

/**
  * @brief  Nastaveni hrany pro triggerovani
  * @param  Trigger edge
  * @retval None
  */
void LA_set_trigger_edge(LA_TRIGGER_EDGE trigEdge);

/**
  * @brief  Vrati stav knihovny osciloskopu
  * @param  None
  * @retval LA state
  */
LA_STATE LA_get_state(void);							

/**
  * @brief  Kontrola triggeru a konce mereni pri kazdem odmerenem vzorku (volano z GPIO_IN)
  * @param  None
  * @retval None
  */
void LA_GPIO_IN_request(void);					

/************************ END OF FILE *****************************************/

