/**
  ******************************************************************************
  * @file    scpi.c
  * @author  PTO Team
  * @version V1.0.0
  * @date    25/3/2013
  * @brief   Knihovna parsujici prikazy	
  ******************************************************************************  
  */ 

/* Includes ------------------------------------------------------------------*/
//#include "<procesor>.h"
#include "stm32f10x.h"
#include "comm.h"
#include "scpi.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/ 
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


/**
 * @brief  Pokusi se v bufferu najit prikaz, rozpoznat ho a vratit
 * @param  Memory range
 * @retval Command
 */    
COMM_CMD SCPI_try_parse_cmd(uint32_t * memory, uint16_t size)
{
	COMM_CMD result;
	//TODO
	return result;
};

/************************ END OF FILE *****************************************/
