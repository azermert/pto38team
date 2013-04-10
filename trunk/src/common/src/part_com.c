/**
  ******************************************************************************
  * @file    part_com.c
  * @author  PTO Team
  * @version V1.0.0
  * @date    25/3/2013
  * @brief   Knihovna parsujici prikazy	
  ******************************************************************************  
  */ 

/* Includes ------------------------------------------------------------------*/
//#include "<procesor>.h"
#include "stm32f0xx.h"
#include "comm.h"
#include "part_com.h"

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
	COMM_CMD result; // definovat strukturu
		
	switch ( /* COMMAND_type */ )
      {
         case IDN?:	// identifikace MCU
					 
				 break;
				 
         case GPIO:	// GPIO_params
// 						SETV				0x98AA	
// 						GET?				0x8C93
						break;
				 
         case CONT:	// CONT_params
// 						STRT				0xA7A6 
// 						STOP				0xA79F 
// 						FRQ?				0x9890
            break;
				 
         case GEN_:	// GEN_params
//            TYPE				
// 						AMPL				0x8E9C  
// 						OFFS				0x9599 
// 						DUTY				0x99AD 
// 						FREQ				0x9896 

// 						SINE				0x9C93 
// 						SQRE				0xA497 
// 						TRIA				0xA6A8 
// 						ARBT				0x9396
            break;
				 
         case LOG_:	// LOG__params
// 						EDGE				0x898C 
// 						TYPE				0xAD95 
// 						CHAN				0x8B8F 
// 						FREQ				0x9896
            break;
				 
         case OSC8:	// OSC8_params
// 						TRIG				0xA690 
// 						LEVL				0x91A2 
// 						EDGE				0x898C 
// 	
// 						NORM				0x9D9F 
// 						AUTO				0x96A3 
// 						SING				0x9C95
            break;
				 
         default:	// spatny prikaz
								
				 break;
      }
	
		return result;
};

/************************ END OF FILE *****************************************/
