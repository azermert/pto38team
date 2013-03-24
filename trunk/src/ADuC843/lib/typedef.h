/**
  ******************************************************************************
  * @file    typedef.h
  * @author  PTO Team
  * @version V1.0.0
  * @date    8/3/2013
  * @brief   This file contains definitions of data types.
  ******************************************************************************
*/ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TYPEDEF_H
#define __TYPEDEF_H


typedef	signed		char	int8_t;
typedef	signed		int		int16_t;
typedef signed		long	int32_t;

typedef	unsigned	char	uint8_t;
typedef	unsigned	int		uint16_t;
typedef	unsigned	long	uint32_t;
 
#ifndef __cplusplus
typedef enum {false = 0, true = !false} bool;
#endif

#endif /*__ADuC843_ADC_H */