#ifndef	_TYPEDEF_H_
#define _TYPEDEF_H_

/* Funktions via pointer*/
typedef void (*CALLBACK)(void*);
typedef void (*TICK)(void);

typedef u8 IR_STATE;
typedef u8 SENSOR_STATE;

enum SENSOR_STATE{
	CLEAR = 0,
	OUTPUT_INVALID,
	OUTPUT_VALID  	
};

enum IR_STATE{
	IR_RESTART = 0,	//!< IR vypnuta, priprava dalsiho cyklu
	IR_OFF,			//!< IR vypnuta, scitani slozky okolniho svetla
	IR_SET,			//!< IR se zapina, priprava na soucet druhe slozky
	IR_ON,			//!< IR zapnuta, scitani sumy
};

typedef struct DInpFILTER{
	u8 input;
	bool output;
} DInpFILTER; 

typedef struct VALUES{
	volatile u32	adc_sum;
	volatile u16	adc_last_on;		 //debug	
	volatile u16	adc_last_off;		 //debug
	IR_STATE		ir_State;
	volatile s32	difference;

	DInpFILTER		filter;

} VALUES; 



 #ifndef NULL
  #define NULL 0
 #endif


#endif
