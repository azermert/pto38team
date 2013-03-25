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



/** @defgroup COMMAND_type
  * @{
  */ 
#define GEN_        0x1234
#define LOGA				0x0001
//atd


/** @defgroup GEN_params
  * @{
  */
#define AMPL				0x7895
#define OFFS				0x5644
#define FREQ				0x2165
//atd

/** @defgroup LOGA_params
  * @{
  */
#define EDGE				0x2165
#define CHAN				0x0351
//atd



/* COMM_Exported_Functions */
COMM_CMD SCPI_try_parse_cmd(uint32_t * memory, uint16_t size); /* Pokusi se v bufferu najit prikaz, rozpoznat ho a vratit*/

#endif /*__SCPI_H */

/************************ END OF FILE *****************************************/
