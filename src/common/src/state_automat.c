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
#include "typedefs.h"
#include "state_automat.h"
#include "comm.h"
#include "parse_cmd.h"
#include "scope.h"
#include "generator.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/ 
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern SCOPE_TypeDef gSCOPE;


COMM_CMD* p_cmd;


/* Private functions ---------------------------------------------------------*/
void set_scope_param(void);
void set_genus_param(void);
/**
  * @brief  Jeden rychly krok stavoveho automatu. Resi hlavne mereni a odesilani dat (cca 1ms)
  * @param  None
  * @retval None
  */    
void STATE_tick_fast(){
			measure_Tick();
//TODO
}


/**
  * @brief  Jeden pomaly krok stavoveho automatu. Resi prijem a zpracovani prikazu (cca 100ms)
  * @param  None
  * @retval None
  */    
void STATE_tick_slow(){
	
	p_cmd = COMM_get_command();
	
	switch (p_cmd->COMMAND_type){
		case WID_IDNQM:
			COMM_print(IDN_STRING);	
		break;
		
		case WID_ERRR:
			COMM_print(ERR_STRING);	
		break;
		
		case WID_OSCP:
			set_scope_param();
			COMM_print(OK_STRING);	
		break;
		
		case WID_GENUS:
			#ifndef ADUC843
			set_genus_param();
			COMM_print(OK_STRING);
			#else
			COMM_print(ERR_STRING);
			#endif
		break;
		
		case WID_LOGUS:
			//set_LOGUS_param();
			COMM_print(OK_STRING);	
		break;
		
		case WID_CONT:
			//set_CONT_param();
			COMM_print(OK_STRING);	
		break;
		
		case WID_VOLT:
			//set_VOLT_param();
			COMM_print(OK_STRING);	
		break;
		
		case WID_MEAS:
			//set_MEAS_param();
			COMM_print(OK_STRING);	
		break;
		
		case WID_GPIO:
			//set_GPIO_param();
			COMM_print(OK_STRING);	
		break;
		
		default:
		break;
	}
}



void set_scope_param(){
	uint8_t prm=0;

		while(p_cmd->PARAM_hash[prm] && prm<3){
			
			switch(p_cmd->PARAM_hash[prm]){
				
			case WID_FREQ:
				if(p_cmd->data_cmd[prm]==WID_1KUSUS){
					SCOPE_set_sample_rate(1000);
				}else if(p_cmd->data_cmd[prm]==WID_2KUSUS){
					SCOPE_set_sample_rate(2000);
				}else if(p_cmd->data_cmd[prm]==WID_5KUSUS){
					SCOPE_set_sample_rate(5000);
				}else if(p_cmd->data_cmd[prm]==WID_10KUS){
					SCOPE_set_sample_rate(10000);
				}else if(p_cmd->data_cmd[prm]==WID_20KUS){
					SCOPE_set_sample_rate(20000);
				}else if(p_cmd->data_cmd[prm]==WID_50KUS){
					SCOPE_set_sample_rate(50000);
				}else if(p_cmd->data_cmd[prm]==WID_100K){
					SCOPE_set_sample_rate(100000);
				}else if(p_cmd->data_cmd[prm]==WID_200K){
					SCOPE_set_sample_rate(200000);
				}else if(p_cmd->data_cmd[prm]==WID_500K){
					SCOPE_set_sample_rate(500000);
				}else if(p_cmd->data_cmd[prm]==WID_1MUSUS){
					SCOPE_set_sample_rate(1000000);
				}else if(p_cmd->data_cmd[prm]==WID_2MUSUS){
					SCOPE_set_sample_rate(2000000);
				}else if(p_cmd->data_cmd[prm]==WID_5MUSUS){
					SCOPE_set_sample_rate(5000000);
				}else if(p_cmd->data_cmd[prm]==WID_10MUS){
					SCOPE_set_sample_rate(10000000);
				}
			break;
			
			case WID_TRIG:
				if(p_cmd->data_cmd[prm]==WID_NORM){
					SCOPE_set_trigger_mode(TRIG_NORMAL);
				}else if(p_cmd->data_cmd[prm]==WID_AUTO){
					SCOPE_set_trigger_mode(TRIG_AUTO);
				}else if(p_cmd->data_cmd[prm]==WID_SING){
					SCOPE_set_trigger_mode(TRIG_SINGLE);
				}
			break;
				
			case WID_PRET:
				SCOPE_setPreTrigger(p_cmd->data_cmd[prm]);
			break;	
			
			case WID_LEVL:
				SCOPE_set_trigger_level(p_cmd->data_cmd[prm]);
			break;
			
			case WID_EDGE:
				if(p_cmd->data_cmd[prm]==WID_RISE){
					SCOPE_set_trigger_edge(SCOPE_RISING);
				}else if(p_cmd->data_cmd[prm]==WID_FALL){
					SCOPE_set_trigger_edge(SCOPE_FALLING);
				}
			break;
				
			case WID_DEPT:
				//TODO zmena bitoveho rozsahu vzorku
			break;
			
			case WID_SRAT:
				SCOPE_start_meas();
			break;
			
			case WID_STOP:
				SCOPE_stop_meas();
			break;
				
			default:
				break;	
			}
		prm++;		
		}
}


void set_genus_param(){
	#ifndef ADUC843
	uint8_t prm=0;

		while(p_cmd->PARAM_hash[prm] && prm<3){
			
			switch(p_cmd->PARAM_hash[prm]){
				
			case WID_FREQ:
				gGenSignal.frequency = p_cmd->data_cmd[prm];
			break;
			
			case WID_TYPE:
				if(p_cmd->data_cmd[prm]==WID_SINE){
					gGenSignal.GEN_signalType = GEN_SINE;
				}else if(p_cmd->data_cmd[prm]==WID_SQRE){
					gGenSignal.GEN_signalType = GEN_SQUARE;
				}else if(p_cmd->data_cmd[prm]==WID_TRIA){
					gGenSignal.GEN_signalType = GEN_TRIANGLE;
				}else if(p_cmd->data_cmd[prm]==WID_ARBT){
					gGenSignal.GEN_signalType = GEN_ARB;
				}
			break;
				
			case WID_AMPL:
				gGenSignal.amplitude = p_cmd->data_cmd[prm]/16;
			break;	
			
			case WID_OFFS:
				gGenSignal.offset = p_cmd->data_cmd[prm]/16;
			break;	
			
			case WID_DUTY:
				gGenSignal.duty = p_cmd->data_cmd[prm]/655;
			break;
				
			case WID_SRAT:
				GEN_start();
			break;
			
			case WID_STOP:
				GEN_stop();
			break;
				
			default:
				break;	
			}
		prm++;		
		}
		#endif
}

/************************ END OF FILE *****************************************/
