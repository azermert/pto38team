/**
  ******************************************************************************
  * @file    comm.c
  * @author  PTO Team
  * @version V1.0.0
  * @date    11/3/2013
  * @brief   Komunikacni knihovna zajistujici inicializaci UART nebo USB a 
  *     funkce typu send, read a spravu bufferu. 
  *     Bude scpi knihovnu zadat o parsovani prikazu
  *     COMM_tick() - podpurna fce pro uart, ktery pomoci ni muze zadat obsluhu (napr plny buffer)  
  ******************************************************************************  
  */ 

/* Includes ------------------------------------------------------------------*/
#include "<procesor>.h"
#include "uart.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/ 
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
COMM_Buffer OutputBuffer;
COMM_Buffer InputBuffer;
COMM_InitTypeDef COMM_desc;
COMM_STATE commState;
struct COMM_CMD commCommand;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
 * @brief  Inicializace pomoci decsriptoru
 * @param  None
 * @retval None
 */ 
void COMM_init(struct COMM_InitTypeDef * p_COMM_desc);        

/**
 * @brief  Vrati stav prichoziho bufferu 
 * @param  None
 * @retval None
 */ 
BUFF_STATE COMM_get_in_buff_state()
{
  //TODO
  return InputBuffer.inPointer;
};    

/**
 * @brief  Vrati stav odchoziho bufferu
 * @param  None
 * @retval None
 */ 
BUFF_STATE COMM_get_out_buff_state()
{
  //TODO
  return OutputBuffer.outPointer;
};     

/**
 * @brief  Vrati pocet volnych byte v prichozim bufferu 
 * @param  None
 * @retval None
 */ 
uint16_t COMM_get_in_free_space()
{
  //TODO
  return 0;
};      

/**
 * @brief  Vrati pocet volnych byte v odchozim bufferu 
 * @param  None
 * @retval None
 */ 
uint16_t COMM_get_out_free_space()
{
  //TODO
  return 0;
};      

/**
 * @brief  Vrati stav knihovny
 * @param  None
 * @retval None
 */ 
COMM_STATE COMM_get_state()
{
  //TODO
  return commState;
};             

/**
 * @brief  Ulozi char na vystupni buffer 
 * @param  None
 * @retval None
 */ 
void COMM_put_char(char c)
{
  //TODO
};            

/**
 * @brief  Ulozi znak na vystupni buffer
 * @param  None
 * @retval None
 */ 
void COMM_put_char(uint8_t c)
{
  //TODO
};          

/**
 * @brief  Prekopiruje zadanou pamet na vystupni buffer 
 * @param  None
 * @retval None
 */ 
void COMM_send(uint32_t * memory, uint16_t size)
{
  //TODO
};  

/**
 * @brief  Prekopiruje vstupni buffer na pridelenou pamet a vrati kolik skutence bylo prekopirovano
 * @param  None
 * @retval None
 */ 
uint16_t COMM_read(uint32_t * memory, uint16_t size)
{
  //TODO
  return 0;
};

/**
 * @brief  Precte znak z prichoziho bufferu
 * @param  None
 * @retval None
 */ 
uint8_t COMM_read_char()
{
  //TODO
  return 0;
};            

/**
 * @brief  Zavola scpi/nasi knihovnu na rozpoznani prikazu a vrati prikaz z vstupniho bufferu
 * @param  None
 * @retval None
 */ 
struct COMM_CMD * COMM_get_command()
{
  //TODO
  return &commCommand;
}; 

/**
 * @brief  Uart zada o obsluhu (plny buffer/chyba atd)
 * @param  None
 * @retval None
 */ 
void COMM_tick()
{
  //TODO
};                    


/************************ END OF FILE *****************************************/
