/**
  ******************************************************************************
  * @file    part_com.h
  * @author  PTO Team
  * @version V1.0.0
  * @date    11/3/2013
  * @brief   This file contains all the functions prototypes for the SCPI firmware 
  *          library.   
  ******************************************************************************
*/ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __PART_COM_H
#define __PART_COM_H

/* Includes ------------------------------------------------------------------*/
/* Zadne includy zde nebudou!!!*/



/** @defgroup COMMAND_type
  * @{
  */ 
#define IDN?				0x0000
#define GPIO				0x0000
#define CONT				0x0000
#define GEN_        0x0000
#define LOG_				0x0000
#define OSC8				0x0000

/** @defgroup GPIO_params
  * @{
  */
#define SETV				0x0000
#define GET?				0x0000

/** @defgroup CONT_params
  * @{
  */
#define STRT				0x0000
#define STOP				0x0000
#define FRQ?				0x0000

/** @defgroup GEN_params
  * @{
  */
#define TYPE				0x0000
#define AMPL				0x0000
#define OFFS				0x0000
#define DUTY				0x0000
#define FREQ				0x0000

#define SINE				0x0000
#define SQRE				0x0000
#define TRIA				0x0000
#define ARBT				0x0000

/** @defgroup LOG__params
  * @{
  */
#define EDGE				0x0000
#define TYPE				0x0000
#define CHAN				0x0000
#define FREQ				0x0000

/** @defgroup OSC8_params
  * @{
  */
#define TRIG				0x0000
#define LEVL				0x0000
#define EDGE				0x0000

#define NORM				0x0000
#define AUTO				0x0000
#define SING				0x0000

/* COMM_Exported_Functions */
COMM_CMD SCPI_try_parse_cmd(uint32_t * memory, uint16_t size); /* Pokusi se v bufferu najit prikaz, rozpoznat ho a vratit*/

#endif /*__PART_COM_H */

/************************ END OF FILE *****************************************/
