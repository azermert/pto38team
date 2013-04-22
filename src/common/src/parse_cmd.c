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

#include "timeBase.h"



/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/ 
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

WORD_ID hash;

COMM_CMD result;
bool error = FALSE;

/* Private function prototypes -----------------------------------------------*/
void parse_OSCP_cmd(void);
void parse_GENUS_cmd(void);
void parse_LOGUS_cmd(void);
void parse_CONT_cmd(void);
void parse_VOLT_cmd(void);
void parse_MEAS_cmd(void);
void parse_GPIO_cmd(void);
WORD_ID read_COMM_hash(void);
void clearCMD(void);

/* Private functions ---------------------------------------------------------*/
void testTimeout(uint8_t _znak){
		uint32_t timeout = actualTime() + 1000000;	// nastaveni timeout
		while(COMM_get_bytes_available() < _znak)
		{
			if(timeElapsed(timeout)){
				break;
			}
		};
}


/**
 * @brief  Pokusi se v bufferu najit prikaz, rozpoznat ho a vratit
 * @param  Memory range
 * @retval Command
 */    
COMM_CMD SCPI_try_parse_cmd(void)
{
		error=FALSE;
		clearCMD();
		if( COMM_get_bytes_available() > 0){
		testTimeout(4);
		hash=read_COMM_hash();
			

		switch(hash){
			
			case WID_IDNQM:
				result.COMMAND_type=hash;
			break;
			
			case WID_ERRR:
				result.COMMAND_type=hash;
			break;
			
			case WID_OSCP:  
				result.COMMAND_type=hash;				
				parse_OSCP_cmd();
			break;
			
			case WID_GENUS:   
				result.COMMAND_type=hash;				
				parse_GENUS_cmd();
			break;
			
			case WID_GPIO:   
				result.COMMAND_type=hash;				
				parse_GPIO_cmd();
			break;
			
			case WID_LOGUS:   
				result.COMMAND_type=hash;				
				parse_LOGUS_cmd();
			break;
			
			case WID_CONT:   
				result.COMMAND_type=hash;				
				parse_CONT_cmd();
			break;
			
			case WID_MEAS:   
				result.COMMAND_type=hash;				
				parse_MEAS_cmd();
			break;
						
			case WID_VOLT:   
				result.COMMAND_type=hash;				
				parse_VOLT_cmd();
			break;
			default:
				error=TRUE;
				break;	
		}		
	}
	if(error==TRUE){
		result.COMMAND_type=WID_ERRR;
		error=FALSE;
}
		return result;
}

//--------------------------------------- OSCILOSKOP

void parse_OSCP_cmd(){
	uint8_t params=0;
	while(COMM_read_char() == ':' && params<3){
		testTimeout(4);
		hash=read_COMM_hash();
				switch(hash){
			case WID_TRIG:
				testTimeout(4);
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
				testTimeout(4);
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
				testTimeout(4);
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
				testTimeout(4);
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
				testTimeout(4);
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
				testTimeout(4);
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

//--------------------------------------- GENERATOR

void parse_GENUS_cmd(){
	uint8_t params=0;
	while(COMM_read_char() == ':' && params<3){
		testTimeout(4);
		hash=read_COMM_hash();
		
		switch(hash){
			case WID_TYPE:
				testTimeout(4);
				if(COMM_read_char() == ' '){
					result.PARAM_hash[params]=hash;
					hash=read_COMM_hash();
					if(IS_GENUS_TYPE(hash)){
						result.data[params]=hash;
					}else{
						error=TRUE;
					}
				}
			break;
			case WID_AMPL:
				testTimeout(4);
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
			case WID_OFFS:
				testTimeout(4);
				if(COMM_read_char() == ' '){
					uint16_t res_over;
					uint32_t value;
					result.PARAM_hash[params]=hash;
					hash=read_COMM_hash();
					res_over = hash & 0x0FFFF;
					value = (hash >> 16) & 0x0FFFF;
					if(res_over == 0){
						result.data[params]=value;
					}else{
						error=TRUE;
					}
				}
			break;
			case WID_DUTY:
				testTimeout(4);
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
			case WID_FREQ:
				testTimeout(4);
				if(COMM_read_char() == ' '){
					result.PARAM_hash[params]=hash;
					hash=read_COMM_hash();
					if(hash<65536){//Zvyseni hodnoty (0-4) G
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
			case WID_FRQQM:
					result.PARAM_hash[params]=hash;
			break;
			case WID_PLSQM:
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
//--------------------------------------- LOGICKZ ANALYZATOR
void parse_LOGUS_cmd(){
	uint8_t params=0;
	while(COMM_read_char() == ':' && params<3){
		testTimeout(4);
		hash=read_COMM_hash();
		
		switch(hash){
			case WID_EDGE:
				testTimeout(4);
				if(COMM_read_char() == ' '){
					result.PARAM_hash[params]=hash;
					hash=read_COMM_hash();
					if(IS_LOGUS_EDGE(hash)){
					result.data[params]=hash;
					}else{
						error=TRUE;
					}
				}
			break;
			
			case WID_TYPE:
				testTimeout(4);
				if(COMM_read_char() == ' '){
					result.PARAM_hash[params]=hash;
					hash=read_COMM_hash();
					if(IS_LOGUS_TYPE(hash)){
						result.data[params]=hash;
					}else{
						error=TRUE;
					}
				}
			break;

			case WID_CHAN:
				testTimeout(4);
				if(COMM_read_char() == ' '){
					result.PARAM_hash[params]=hash;
					hash=read_COMM_hash();
					if(hash == 1 || hash == 0){//Zvyseni hodnoty (0-4) G
						result.data[params]=hash;
					}else{
						error=TRUE;
					}
				}
			break;		

			case WID_FREQ:
				testTimeout(4);
				if(COMM_read_char() == ' '){	
					result.PARAM_hash[params]=hash;
					hash=read_COMM_hash();
					if(IS_LOGUS_FREQ(hash)){
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

//--------------------------------------- CITAC

void parse_CONT_cmd(){
	uint8_t params=0;
	while(COMM_read_char() == ':' && params<3){
		testTimeout(4);
		hash=read_COMM_hash();
		
		switch(hash){
			case WID_STRT:
					result.PARAM_hash[params]=hash;
			break;
			
			case WID_STOP:
					result.PARAM_hash[params]=hash;
			break;
			
			case WID_FRQQM:
				testTimeout(4);
				if(COMM_read_char() == ' '){
					result.PARAM_hash[params]=hash;
					hash=read_COMM_hash();
					if(IS_LOGUS_EDGE(hash)){
					result.data[params]=hash;
					}else{
						error=TRUE;
					}
				}
			break;
			
			default:
				error=TRUE;
				break;	
		}	
	params++;
	}
	COMM_read_char();
}
//--------------------------------------- VOLTMETR - VOLT
void parse_VOLT_cmd(){
	uint8_t params=0;
	while(COMM_read_char() == ':' && params<3){
		testTimeout(4);
		hash=read_COMM_hash();
		
		switch(hash){
			case WID_VALQM:
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
//--------------------------------------- VOLTMETR - MEAS
void parse_MEAS_cmd(){
	uint8_t params=0;
	while(COMM_read_char() == ':' && params<3){
		testTimeout(4);
		hash=read_COMM_hash();
		
		switch(hash){
			case WID_VALQM:
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
//--------------------------------------- GPIO
void parse_GPIO_cmd(){
	uint8_t params=0;
	while(COMM_read_char() == ':' && params<3){
		testTimeout(4);
		hash=read_COMM_hash();
		
		switch(hash){
			case WID_SETV:
					result.PARAM_hash[params]=hash;
			break;	
			case WID_GETQM:
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
//---------------------------------------

WORD_ID read_COMM_hash(void){
	uint8_t inField[4];
	COMM_read((uint8_t*)&inField, 4);
	return (inField[3] << 24)|(inField[2] << 16)|(inField[1] << 8)|(inField[0]);
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
