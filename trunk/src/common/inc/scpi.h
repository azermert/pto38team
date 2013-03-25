/**
  ******************************************************************************
  * @file    comm.h
  * @author  PTO Team
  * @version V1.0.0
  * @date    11/3/2013
  * @brief   This file contains all the functions prototypes for the SCPI firmware 
  *          library.   
  ******************************************************************************
*/ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SCPI_H
#define __SCPI_H

/* Includes ------------------------------------------------------------------*/
/* Zadne includy zde nebudou!!!*/

typedef enum                              
{
  SCPI_MEAS = 0,
  SPCI_SET,
  SCPI_LOAD
} SCPI_CMD;

typedef enum
{
	SCPI_FREQ = 0,
	SCPI_VOLT,
	SCPI_DUTY,
	SCPI_BYTES
} SCPI_PARAM;


/* COMM_Exported_Functions */
COMM_CMD SCPI_try_parse_cmd(uint32_t * memory, uint16_t size); /* Pokusi se v bufferu najit prikaz, rozpoznat ho a vratit*/

#endif /*__SCPI_H */

/************************ END OF FILE *****************************************/
