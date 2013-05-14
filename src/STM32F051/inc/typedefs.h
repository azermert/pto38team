/**
  ******************************************************************************
  * @file    typedefs.h
  * @author  PTO Team
  * @version V1.0.0
  * @date    12/4/2013
  * @brief   Pouze include definic
  ******************************************************************************
*/ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TYPES_H
#define __TYPES_H

#include "stm32f0xx.h"
#define IDN_STRING "IDN_STM32F051 Multipurpose device V1.0"
#define ERR_STRING "Some problem with command parsing (chceck syntax and values)" //60chars
#define OK_STRING  "ACKN"

typedef enum {FALSE = 0, TRUE = !FALSE} bool;

typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned char  u8;
typedef signed int s32;
typedef signed short s16;
typedef signed char  s8;

#endif /*__TYPES_H */

/************************ END OF FILE *****************************************/
