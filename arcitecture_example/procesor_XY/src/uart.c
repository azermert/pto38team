/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "uart.h"



/* Privete variables definitions ---------------------------------------------*/

uint16_t	RxInPointer = 0;
uint16_t	RxOutPointer = 0;
uint8_t		RxBuffer[UART_BUFF_SIZE];
BUFFER_STATUS RxBufferState;

uint16_t	TxInPointer = 0;
uint16_t	TxOutPointer = 0;
uint8_t		TxBuffer[UART_BUFF_SIZE];
BUFFER_STATUS TxBufferState;




/* Private function prototypes -----------------------------------------------*/



/* Functions declarations ----------------------------------------------------*/
/**
  * @brief  This function stores char to TxBuffer.
  * @retval None
  */
void UART_send(char c)
{
}

/**
  * @brief  This function stores n chars from memory to TxBuffer.
  * @retval None
  */
void UART_send_strinf(uint32_t *p_memory, uint32_t nBytes)
{
}

/**
  * @brief  This function returns char from RxBuffer.
  * @retval None
  */
char UART_get_char(void)
{
	return 0;
}

/**
  * @brief  This function returns TxBuffer state.
  * @retval BUFFER_STATUS
  */
BUFFER_STATUS UART_get_TxBuff_state()
{
	return BUFF_ERR;
}

/**
  * @brief  This function returns TxBuffer state.
  * @retval BUFFER_STATUS
  */
BUFFER_STATUS UART_get_RxBuff_state()
{
	return BUFF_ERR;
}

/**
  * @brief  Initialization of UART.
  * @retval None
  */
void UART_init(BAUDRATE baudRate, STOPBIT stopBits, PARITY parity)
{
}

/**
  * @brief  Initialization of UART.
  * @retval None
  */
void UART_init_struct(UART_TypeDef *p_uartHandler_s)
{
}


/**
  * @brief  This function handles UART Handler.
  * @retval None
  */
void UART_IRQ_handler()
{
}
