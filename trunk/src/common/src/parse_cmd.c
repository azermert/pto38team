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

WORD_ID hash;

COMM_CMD result;
bool error = FALSE;

/* Private function prototypes -----------------------------------------------*/
void parse_OSCP_cmd(void);
WORD_ID read_COMM_hash(void);
void clearCMD(void);

/* Private functions ---------------------------------------------------------*/



/**
 * @brief  Pokusi se v bufferu najit prikaz, rozpoznat ho a vratit
 * @param  Memory range
 * @retval Command
 */    
COMM_CMD SCPI_try_parse_cmd(void)
{
		error=FALSE;
		clearCMD();
		if( COMM_get_bytes_available() > 3){		
		hash=read_COMM_hash();

		switch(hash){
			
			case WID_IDNQM:
				result.COMMAND_type=hash;
			break;
			
			case WID_ERRR:   
				//parse_E_cmd();
				result.COMMAND_type=hash;
			break;
			
			case WID_OSCP:   
				parse_OSCP_cmd();
				result.COMMAND_type=hash;
			break;
			
			case WID_GENUS:   
				//parse_OSC_cmd();
				result.COMMAND_type=hash;
			break;
			
			case WID_GPIO:   
				//parse_OSC_cmd();
				result.COMMAND_type=hash;
			break;
			
			case WID_LOGUS:   
				//parse_OSC_cmd();
				result.COMMAND_type=hash;
			break;
			
			case WID_CONT:   
				//parse_OSC_cmd();
				result.COMMAND_type=hash;
			break;
			
			case WID_MEAS:   
				//parse_OSC_cmd();
				result.COMMAND_type=hash;
			break;
			
			case WID_VOLT:   
				//parse_OSC_cmd();
				result.COMMAND_type=hash;
			break;
			
			default:
				break;	
		}		
	}
	if(error==TRUE){
		result.COMMAND_type=WID_ERRR;
		error=FALSE;
}
		return result;
}



void parse_OSCP_cmd(){
	uint8_t params=0;
	while(COMM_read_char() == ':' && params<3){
		while(COMM_get_bytes_available() < 4){};
		hash=read_COMM_hash();
		
		switch(hash){
			case WID_TRIG:
				while(COMM_get_bytes_available() < 6){};
				if(COMM_read_char() == ' '){
					result.PARAM_hash[params]=hash;
					hash=read_COMM_hash();
					if(IS_OSCP_TRIG(hash)){
						result.data[params]=hash;
					}else{
						error=TRUE;
					}
				}
			break;
			case WID_PRET:
				while(COMM_get_bytes_available() < 6){};
				if(COMM_read_char() == ' '){
					result.PARAM_hash[params]=hash;
					hash=read_COMM_hash();
					if(hash<65536){
						result.data[params]=hash;
					}else{
						error=TRUE;
					}
				}
			break;
			case WID_LEVL:
				while(COMM_get_bytes_available() < 6){};
				if(COMM_read_char() == ' '){
					result.PARAM_hash[params]=hash;
					hash=read_COMM_hash();
					if(hash<65536){
						result.data[params]=hash;
					}else{
						error=TRUE;
					}
				}
			break;
			case WID_EDGE:
				while(COMM_get_bytes_available() < 6){};
				if(COMM_read_char() == ' '){
					result.PARAM_hash[params]=hash;
					hash=read_COMM_hash();
					if(IS_OSCP_EDGE(hash)){
					result.data[params]=hash;
					}else{
						error=TRUE;
					}
				}
			break;
			case WID_FREQ:
				while(COMM_get_bytes_available() < 6){};
				if(COMM_read_char() == ' '){	
					result.PARAM_hash[params]=hash;
					hash=read_COMM_hash();
					if(IS_OSCP_FREQ(hash)){
						result.data[params]=hash;
					}else{
						error=TRUE;
					}
				}
			break;
			case WID_DEPT:
				while(COMM_get_bytes_available() < 6){};
				if(COMM_read_char() == ' '){
					result.PARAM_hash[params]=hash;
					hash=read_COMM_hash();
					if(IS_OSCP_DEPT(hash)){
						result.data[params]=hash;
					}else{
						error=TRUE;
					}
				}
			break;
			case WID_STRT:
					result.PARAM_hash[params]=hash;
			break;
			case WID_STOP:
					result.PARAM_hash[params]=hash;
			break;	
			default:
				error=TRUE;
				break;	
		}	
	params++;
	}
	COMM_read_char();
}

WORD_ID read_COMM_hash(void){
	uint8_t inField[4];
	COMM_read((uint8_t*)&inField, 4);
//  	return inField[3]+256*inField[2]+256*256*inField[1]+256*256*256*inField[0];	//?????
	return (inField[0] << 24)|(inField[1] << 16)|(inField[2] << 8)|(inField[3]);	//?????
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
