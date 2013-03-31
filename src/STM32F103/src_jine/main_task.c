
#include "main_task.h"
#include "IO.h"
#include "timeBase.h"

#include "globals.h"

#define MEASURE_PERIOD 20000;	//20ms

typedef struct MEASURE{
	u32		start_ir_off;
	u32		value_ir_off;
	u32		start_ir_on;
	u32		value_ir_on;
	u32		period;
} MEASURE;

MEASURE measure;


void real_Tick(void){

}



void fast_Tick(void){		//spusteno 2x kazdou milisekundu

	

}

void filter_Update(struct DInpFILTER * _flt, bool _bit){
#define LENGHT 3
#define MASK 0x07  //	'B'0000 0111

	_flt->input = _flt->input / 2;	//realizace posunu o 1 bit
	if(_bit){
		_flt->input |= (1 << (LENGHT-1) );
	}

	if(_flt->input == 0){
		_flt->output = FALSE;
	}else if(_flt->input == MASK){
		_flt->output = TRUE;
	}

}

u32 getResult(MEASURE m){
	return  - m.value_ir_on + m.start_ir_on + m.value_ir_off - m.start_ir_off;
}

void slow_Tick(void){	  	//spusteno 5x za sekundu
	DInpFILTER * flt = (DInpFILTER*)(&gValues.filter);

//IR_RESTART:
			setOutputState(IR_LED, 0);
			gValues.adc_sum = 0;
			gValues.ir_State = IR_OFF;
			measure.period = actualTime() + MEASURE_PERIOD;
			measure.start_ir_off = gValues.adc_sum;
//IR_OFF:
			while(!(timeElapsed(measure.period)));
			measure.value_ir_off = gValues.adc_sum;
			gValues.ir_State = IR_SET;
//IR_SET:
			setOutputState(IR_LED, 1);
			gValues.ir_State = IR_ON;
			measure.period = actualTime() + MEASURE_PERIOD;
			measure.start_ir_on = gValues.adc_sum;
//IR_ON:
			while(!(timeElapsed(measure.period)));
			measure.value_ir_on = gValues.adc_sum;
			gValues.ir_State = IR_RESTART;
			setOutputState(IR_LED, 0);
	

			gValues.difference = getResult(measure);
			
			if( gValues.difference < -4000 ){
				GPIO_WriteBit(GPIOC, GPIO_Pin_8, (BitAction)1);
				filter_Update(flt, TRUE);				
			}else{
				GPIO_WriteBit(GPIOC, GPIO_Pin_8, (BitAction)0);
				filter_Update(flt, FALSE);
			}





//	static bool LED_state = FALSE;
//	static u32 LED_time = 0;
//
//	if (LED_time == 0){
//		LED_time = actualTime() + 600000;
//	}

//	if(timeElapsed(LED_time)){
//		LED_time += 600000;
//		if(LED_state){
//			setOutputState(IR_LED, 0);
//			LED_state = FALSE;	
//		}else{
//			setOutputState(IR_LED, 1);
//			LED_state = TRUE;
//		}
//	}	

}
