#ifndef _COMM_H
#define _COMM_H

#include "misc.h"
#include "globals.h"

#define BUFFER_SIZE 	1024	  // <type u16

/* User defined values */
struct COMM_DESC;
typedef void(*cmTICK)(struct COMM_DESC * _desc);
typedef void(*cmPutChar)(struct COMM_DESC * _desc, char _ch);
typedef char(*cmGetChar)(struct COMM_DESC * _desc);


typedef u8 BUFFER_STATUS;
typedef u8 BUFFER_KEY;
typedef u8 MSG_READ_STATE;
typedef u8 MSG_READ_ERROR;
typedef u8 COMM_ERROR;

enum BUFFER_STATUS{
	BUFF_RDY = 0,			//ready to send / ready to read  - tento buffer je zacatek zpravy
	BUFF_FREE,				//buffer neni obsazen, je mozno ho pouzit
	BUFF_FULL
};

enum COMM_ERROR{
	CM_NO_ERROR = 0,
	CM_RCV_OVERFLOW,
	CM_SND_LOCKED
};

enum MSG_READ_STATE{
	MSG_RD_IDLE = 0,
	MSG_CNT_READ,		//read by counter
	MSG_SEP_READ		//read by separator
};

enum MSG_READ_ERROR{
	MSG_NO_ERROR = 0,
	MSG_RD_BUSY
};

typedef struct BUFFER{
	char	buffer[BUFFER_SIZE];
	char	*ptr_end;
	char	*ptr_read;
	char	*ptr_write;
	BUFFER_STATUS Status;
} BUFFER;

typedef struct MSG_READ_CTX{
	struct BUFFER * source;
	char *			sink;
	CALLBACK		cb;
	void*			cbData;
	u16				size;
	char			separator;
	MSG_READ_STATE State;
} MSG_READ_CTX;

typedef struct COMM_DESC{
 	cmTICK		Tick;
	cmPutChar 	putChar;
	cmGetChar	getChar;

	COMM_ERROR 	Error;
	bool		isSend;		//<! Priznak zpravy cekajici na odeslani

	struct BUFFER	sndBuff;
	struct BUFFER	rcvBuff;

	struct MSG_READ_CTX 	msgRd_ctx;

	BUFFER_KEY		bLock;		//<! Uzamceni vysilaciho bufferu (0=volny)

} COMM_DESC;

/*typedef struct MESSAGE{
	char *	msg;
	u16		size;
	//HW_ID	id;	   ?
} MESSAGE;	 */

/* Exported function prototypes */
BUFFER_KEY buffer_Lock(struct COMM_DESC * _desc);	   				//zakaze pridelovani snd buferu
void buffer_unLock(struct COMM_DESC * _desc, BUFFER_KEY _key);		//povoli pridelovani snd bufferu

void sendMessage(struct COMM_DESC * _desc, char * _msg, u16 _size , CALLBACK * _cb, void* _cbData);

MSG_READ_ERROR readMessage(struct COMM_DESC * _desc, char * _msg , char _separator , CALLBACK _cb , void* _cbData);
MSG_READ_ERROR readBinaryMessage(struct COMM_DESC * _desc, char * _msg , u16 _size , CALLBACK _cb, void* _cbData);

COMM_ERROR sendShortMessage(struct COMM_DESC * _desc, char * _msg, u16 _size);
COMM_ERROR sendShortMessage_Lock(struct COMM_DESC * _desc, char * _msg, u16 _size, BUFFER_KEY _key);

void init_Comm(struct COMM_DESC * _desc);


#endif
