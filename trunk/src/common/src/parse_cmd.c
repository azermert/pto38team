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

#include "comm.h"
#include "parse_cmd.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/ 
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

uint32_t hash;

COMM_CMD result;
bool error = FALSE;

/* Private function prototypes -----------------------------------------------*/
void parse_OSC_cmd(void);
uint32_t read_COMM_hash(void);
void clearCMD(void);

/* Private functions ---------------------------------------------------------*/



/**
 * @brief  Pokusi se v bufferu najit prikaz, rozpoznat ho a vratit
 * @param  Memory range
 * @retval Command
 */    
COMM_CMD SCPI_try_parse_cmd()
{
		error=FALSE;
	
		if( COMM_get_bytes_available() > 3){		
		hash=read_COMM_hash();
		clearCMD();
		switch(hash){
			case IDN:
				COMM_print(IDN_STRING);
			break;
			case OSC:
				parse_OSC_cmd();
				result.COMMAND_type=OSC;
			break;
			default:
				break;	
		}		
	}
	if(error==TRUE){
		error=FALSE;
}
		return result;
}



void parse_OSC_cmd(){
	uint8_t params=0;
	while(COMM_read_char() == ':' && params<3){
		while(COMM_get_bytes_available() < 4){};
		hash=read_COMM_hash();
		
		switch(hash){
			case TRIG:
				while(COMM_get_bytes_available() < 6){};
				if(COMM_read_char() == ' '){
					result.PARAM_hash[params]=TRIG;
					hash=read_COMM_hash();
					if(IS_OSC_TRIG(hash)){
						result.data[params]=hash;
					}else{
						error=TRUE;
					}
				}
			break;
			case LEVL:
				while(COMM_get_bytes_available() < 6){};
				if(COMM_read_char() == ' '){
					result.PARAM_hash[params]=LEVL;
					hash=read_COMM_hash();
					if(hash<65536){
						result.data[params]=hash;
					}else{
						error=TRUE;
					}
				}
			break;
			case EDGE:
				while(COMM_get_bytes_available() < 6){};
				if(COMM_read_char() == ' '){
					result.PARAM_hash[params]=EDGE;
					hash=read_COMM_hash();
					if(IS_EDGE(hash)){
						result.data[params]=hash;
					}else{
						error=TRUE;
					}
				}
			break;
			case FREQ:
				while(COMM_get_bytes_available() < 6){};
				if(COMM_read_char() == ' '){
					result.PARAM_hash[params]=FREQ;
					hash=read_COMM_hash();
					if(IS_OSC_FREQ(hash)){
						result.data[params]=hash;
					}else{
						error=TRUE;
					}
				}
			break;
			case DEPT:
				while(COMM_get_bytes_available() < 6){};
				if(COMM_read_char() == ' '){
					result.PARAM_hash[params]=DEPT;
					hash=read_COMM_hash();
					if(IS_OSC_DEPT(hash)){
						result.data[params]=hash;
					}else{
						error=TRUE;
					}
				}
			break;
			case STRT:
					result.PARAM_hash[params]=STRT;
			break;
			case STOP:
					result.PARAM_hash[params]=STOP;
			break;	
			default:
				error=TRUE;
				break;	
		}	
	params++;
	}
	COMM_read_char();
}




uint32_t read_COMM_hash(void){
	uint8_t inField[4];
	COMM_read((uint8_t*)&inField, 4);
	return inField[3]+256*inField[2]+256*256*inField[1]+256*256*256*inField[0];

}

void clearCMD(void){
  result.COMMAND_type=0;
	result.PARAM_hash[0]=0;
  result.PARAM_hash[1]=0;
  result.PARAM_hash[2]=0;
  result.data[0]=0;
	result.data[1]=0;
	result.data[2]=0;
}

/************************ END OF FILE *****************************************/
