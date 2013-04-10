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
#define IDN?				0x8D8D	// 49 44 4e 3f
#define GPIO				0x9798	// 47 50 49 4f
#define CONT				0x92A2	// 43 4f 4e 54
#define GEN_        0x8CAD	// 47 45 4e 5f
#define LOG_				0x9BA6	// 4c 4f 47 5f
#define OSC8				0xA27B	// 4f 53 43 38

/** @defgroup GPIO_params
  * @{
  */
#define SETV				0x98AA	// 53 45 54 56
#define GET?				0x8C93	// 47 45 54 3f

/** @defgroup CONT_params
  * @{
  */
#define STRT				0xA7A6 // 53 54 52 54
#define STOP				0xA79F // 53 54 4f 50
#define FRQ?				0x9890 // 46 52 51 3f

/** @defgroup GEN_params
  * @{
  */
#define TYPE				0xAD95 // 54 59 50 45
#define AMPL				0x8E9C // 41 4d 50 4c 
#define OFFS				0x9599 // 4f 46 46 53
#define DUTY				0x99AD // 44 55 54 59
#define FREQ				0x9896 // 46 52 45 51

#define SINE				0x9C93 // 53 49 4e 45
#define SQRE				0xA497 // 53 51 52 45
#define TRIA				0xA6A8 // 54 52 49 41
#define ARBT				0x9396 // 41 52 42 54

/** @defgroup LOG__params
  * @{
  */
#define EDGE				0x898C // 45 44 47 45
//#define TYPE				0xAD95 // 54 59 50 45
#define CHAN				0x8B8F // 43 48 41 4e
//#define FREQ				0x9896 // 46 52 45 51

/** @defgroup OSC8_params
  * @{
  */
#define TRIG				0xA690 // 54 52 49 47
#define LEVL				0x91A2 // 4c 45 56 4c
//#define EDGE				0x898C // 45 44 47 45

#define NORM				0x9D9F // 4e 4f 52 4d
#define AUTO				0x96A3 // 41 55 54 4f
#define SING				0x9C95 // 53 49 4e 47

/* COMM_Exported_Functions */
COMM_CMD SCPI_try_parse_cmd(uint32_t * memory, uint16_t size); /* Pokusi se v bufferu najit prikaz, rozpoznat ho a vratit*/

#endif /*__PART_COM_H */

/************************ END OF FILE *****************************************/
