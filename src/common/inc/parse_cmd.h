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
#define IDN				0x49444e3f	// 49 44 4e 3f
#define GPIO			0x4750494f	// 47 50 49 4f
#define CONT			0x434f4e54	// 43 4f 4e 54
#define GEN       0x47454e5f	// 47 45 4e 5f
#define LOG				0x4c4f475f	// 4c 4f 47 5f
#define OSC	  		0x4f534350	// 4f 53 43 50

#define STRT			0x53545254 // 53 54 52 54
#define STOP			0x53544f50 // 53 54 4f 50

/** @defgroup GPIO_params
  * @{
  */
#define SETV			0x98AA	// 53 45 54 56
#define GET				0x8C93	// 47 45 54 3f

/** @defgroup CONT_params
  * @{
  */
#define FRQ				0x9890 // 46 52 51 3f

/** @defgroup GEN_params
  * @{
  */
#define TYPE				0xAD95 // 54 59 50 45
#define AMPL				0x8E9C // 41 4d 50 4c 
#define OFFS				0x9599 // 4f 46 46 53
#define DUTY				0x99AD // 44 55 54 59
#define FREQ				0x46524551 // 46 52 45 51

#define SINE				0x9C93 // 53 49 4e 45
#define SQRE				0xA497 // 53 51 52 45
#define TRIA				0xA6A8 // 54 52 49 41
#define ARBT				0x9396 // 41 52 42 54

/** @defgroup LOG__params
  * @{
  */
#define EDGE				0x45444745 // 45 44 47 45
//#define TYPE				0xAD95 // 54 59 50 45
#define CHAN				0x8B8F // 43 48 41 4e
//#define FREQ				0x9896 // 46 52 45 51

/** @defgroup OSC8_params
  * @{
  */
#define TRIG				0x54524947 // 54 52 49 47
	#define NORM				0x4e4f524d // 4e 4f 52 4d
	#define AUTO				0x4155544f // 41 55 54 4f
	#define SING				0x53494e47 // 53 49 4e 47
	#define IS_OSC_TRIG(TRG) (((TRG) == NORM) || \
                            ((TRG) == AUTO) || \
														((TRG) == SING))

#define LEVL				0x4c45564c // 4c 45 56 4c

#define EDGE				0x45444745 // 45 44 47 45
	#define RISE				0x52495345 
	#define FALL				0x46414c4c
	#define IS_EDGE(ED) (((ED) == RISE) || \
											((ED) == FALL))
	
#define FREQ				0x46524551 // 46 52 45 51
	#define F_1K				0x314b5f5f
	#define F_2K				0x324b5f5f
	#define F_5K				0x354b5f5f
	#define F_10K				0x31304b5f
	#define F_20K				0x32304b5f
	#define F_50K				0x35304b5f
	#define F_100K			0x3130304b
	#define F_200K			0x3230304b
	#define F_500K			0x3530304b
	#define F_1M				0x314d5f5f
	#define F_2M				0x324d5f5f
	#define F_5M				0x354d5f5f
	#define F_10M				0x31304d5f
	#define IS_OSC_FREQ(FRQ) (((FRQ) == F_1K) || \
                            ((FRQ) == F_2K) || \
														((FRQ) == F_5K) || \
														((FRQ) == F_10K) || \
														((FRQ) == F_20K) || \
														((FRQ) == F_50K) || \
														((FRQ) == F_100K) || \
														((FRQ) == F_200K) || \
														((FRQ) == F_500K) || \
														((FRQ) == F_1M) || \
														((FRQ) == F_2M) || \
														((FRQ) == F_5M) || \
														((FRQ) == F_10M))

	
	
#define DEPT 				0x44455054
#define BIT_8				0x3038425f
#define BIT_16			0x3136425f
#define IS_OSC_DEPT(DEP) (((DEP) == BIT_8) || \
													((DEP) == BIT_16))
	
/* COMM_Exported_Functions */
COMM_CMD SCPI_try_parse_cmd(void); /* Pokusi se v bufferu najit prikaz, rozpoznat ho a vratit*/

#endif /*__PART_COM_H */

/************************ END OF FILE *****************************************/
