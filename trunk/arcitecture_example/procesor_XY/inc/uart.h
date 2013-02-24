#ifndef __procesor_XY_UART_H
#define __procesor_XY_UART_H


#define UART_BUFF_SIZE 128

/* User defined values */

typedef uint8_t BUFFER_STATUS;
typedef uint8_t BAUDRATE;
typedef uint8_t PARITY;
typedef uint8_t STOPBIT;

typedef struct
{
  uint32_t UART_BaudRate;            /*!< This member configures the USART communication baud rate.
                                           The baud rate is computed using the following formula:
                                            - IntegerDivider = ((PCLKx) / (16 * (USART_InitStruct->USART_BaudRate)))
                                            - FractionalDivider = ((IntegerDivider - ((uint32_t) IntegerDivider)) * 16) + 0.5 */

  uint32_t UART_StopBits;            /*!< Specifies the number of stop bits transmitted.
                                           This parameter can be a value of @ref USART_Stop_Bits */

  uint32_t UART_Parity;              /*!< Specifies the parity mode.
                                           This parameter can be a value of @ref USART_Parity
                                           @note When parity is enabled, the computed parity is inserted
                                                 at the MSB position of the transmitted data (9th bit when
                                                 the word length is set to 9 data bits; 8th bit when the
                                                 word length is set to 8 data bits). */
} UART_TypeDef;

enum BUFFER_STATUS{
	BUFF_RDY = 0,			//ready to send / ready to read
	BUFF_FREE,				//buffer neni obsazen, je mozno ho pouzit
	BUFF_FULL,
	BUFF_ERR
};

enum PARITY{
	PARITY_EVEN=0,
	PARITY_ODD,
	PARITY_NONE
};

enum BAUDRATE{
	BAUDRATE_9600=0,
	BAUDRATE_19200,
	BAUDRATE_38400,
	BAUDRATE_57600,
	BAUDRATE_115200,
	BAUDRATE_230400,
	BAUDRATE_460800,
	BAUDRATE_921600
};

enum STOPBIT{
	STOPBIT_1=0,
	STOPBIT_1_5,
	STOPBIT_2
};


void UART_send(char c);
void UART_send_string(uint32_t *p_memory, uint32_t nBytes);

char UART_get_char(void);

BUFFER_STATUS UART_get_output_buff_state(void);
BUFFER_STATUS UART_get_input_buff_state(void);

void UART_init(BAUDRATE baudRate, STOPBIT stopBits, PARITY parity);
void UART_init_struct(UART_TypeDef *p_uartHandler_s);

void UART_IRQ_handler(void);


	
#endif
