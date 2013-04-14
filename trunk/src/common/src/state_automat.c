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


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/ 
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern SCOPE_TypeDef gSCOPE;


COMM_CMD* p_cmd;


/* Private functions ---------------------------------------------------------*/
void set_scope_param(void);
/**
  * @brief  Jeden rychly krok stavoveho automatu. Resi hlavne mereni a odesilani dat (cca 1ms)
  * @param  None
  * @retval None
  */    
void STATE_tick_fast(){
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
		case IDN:
			COMM_print(IDN_STRING);	
		break;
		
		case ERRR:
			COMM_print(ERR_STRING);	
		break;
		
		case OSC:
			set_scope_param();
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
				
			case FREQ:
				if(p_cmd->data[prm]==F_1K){
					SCOPE_set_sample_rate(1000);
				}else if(p_cmd->data[prm]==F_2K){
					SCOPE_set_sample_rate(2000);
				}else if(p_cmd->data[prm]==F_5K){
					SCOPE_set_sample_rate(5000);
				}else if(p_cmd->data[prm]==F_10K){
					SCOPE_set_sample_rate(10000);
				}else if(p_cmd->data[prm]==F_20K){
					SCOPE_set_sample_rate(20000);
				}else if(p_cmd->data[prm]==F_50K){
					SCOPE_set_sample_rate(50000);
				}else if(p_cmd->data[prm]==F_100K){
					SCOPE_set_sample_rate(100000);
				}else if(p_cmd->data[prm]==F_200K){
					SCOPE_set_sample_rate(200000);
				}
			break;
			
			case TRIG:
				if(p_cmd->data[prm]==NORM){
					SCOPE_set_trigger_mode(TRIG_SIGNAL);
				}else if(p_cmd->data[prm]==AUTO){
					SCOPE_set_trigger_mode(TRIG_AUTO);
				}else if(p_cmd->data[prm]==SING){
					SCOPE_set_trigger_mode(TRIG_SINGLE);
				}
			break;
				
			case PRET:
				SCOPE_setPreTrigger(p_cmd->data[prm]);
			break;	
			
			case LEVL:
				SCOPE_set_trigger_level(p_cmd->data[prm]);
			break;
			
			case EDGE:
				if(p_cmd->data[prm]==RISE){
					SCOPE_set_trigger_edge(SCOPE_RISING);
				}else if(p_cmd->data[prm]==FALL){
					SCOPE_set_trigger_edge(SCOPE_FALLING);
				}
			break;
				
			case DEPT:
				//TODO zmena bitoveho rozsahu vzorku
			break;
			
			case STRT:
				SCOPE_start_meas();
			break;
			
			case STOP:
				SCOPE_stop_meas();
			break;
				
			default:
				break;	
			}
		prm++;		
		}
}


/************************ END OF FILE *****************************************/
