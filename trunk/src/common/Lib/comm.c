
#include "comm.h"

/* Private variables*/

/* Function prototypes */
bool bufferStore(struct BUFFER * _buff, char _ch);
char bufferRead(struct BUFFER * _buff);

/* Routines */

void msgReadTick(struct MSG_READ_CTX * _ctx){
while(1){
	switch(_ctx->State){
		case MSG_CNT_READ:
			if(_ctx->source->Status == BUFF_FREE){
				//neni co cist
				return;
			}
			*_ctx->sink = bufferRead(_ctx->source);
			_ctx->sink++;
			_ctx->size--;
			if(_ctx->size == 0){
				//cteni dokonceno
				_ctx->State = MSG_RD_IDLE;
				_ctx->cb(NULL);
				return;
			}
			break;

		case MSG_SEP_READ:
			if(_ctx->source->Status == BUFF_FREE){
				//neni co cist
				return;
			}
			*_ctx->sink = bufferRead(_ctx->source);
			_ctx->sink++;
			if(*_ctx->sink == _ctx->separator){
				//cteni dokonceno
				_ctx->State = MSG_RD_IDLE;
				_ctx->cb(_ctx->cbData);
				return;
			}
			break;

		case MSG_RD_IDLE:
		default:
			return;	
	}//endswitch
}//endwhile	
}

void COMM_Tick(struct COMM_DESC * _desc){

	//kontrola stavu, spuštìní rutin, error handler (TODO)
	//kontrola timeoutù (pokud by se implementovaly)

	if(_desc->sndBuff.Status != BUFF_FREE){
		_desc->isSend = TRUE;
	}

 	msgReadTick(&_desc->msgRd_ctx);	
}


void putChar(struct COMM_DESC * _desc, char _ch){	  //prijeti bytu z nizsi vrstvy
	if(_desc->rcvBuff.Status != BUFF_FULL){
		bufferStore(&_desc->rcvBuff, _ch);
	}else{
		_desc->Error = CM_RCV_OVERFLOW;
	}				
}


char getChar(struct COMM_DESC * _desc){	  //prevzeti bytu do nizsi vrstvy
	char retVal = bufferRead(&_desc->sndBuff);
	if(_desc->sndBuff.Status == BUFF_FREE){
		_desc->isSend = FALSE;
	}
	return retVal;			
}



BUFFER_KEY getKey(struct COMM_DESC * _desc){
	static BUFFER_KEY retVal;
	retVal++;
	return retVal;
}

BUFFER_KEY buffer_Lock(struct COMM_DESC * _desc){		//zakaze vlozeni do buferu
	if(_desc->bLock != 0){
		return (BUFFER_KEY)0;			//jiz zamceno
	}
	_desc->bLock = getKey(_desc);		//ziskani unikatniho klice
	return _desc->bLock;
}

void buffer_unLock(struct COMM_DESC * _desc, BUFFER_KEY _key){		//povoli vlozeni do bufferu
	if(_key == _desc->bLock){
		_desc->bLock = 0;	//klic se shoduje, odemceno
	}
}



MSG_READ_ERROR readMessage(struct COMM_DESC * _desc, char * _msg , char _separator , CALLBACK _cb , void* _cbData){
	MSG_READ_CTX * ctx = &_desc->msgRd_ctx;

	if(ctx->State != MSG_RD_IDLE) return MSG_RD_BUSY;

	ctx->source = &_desc->rcvBuff;
	ctx->sink = _msg;
	ctx->separator = _separator;
	
	ctx->State = MSG_SEP_READ;
	ctx->cb = _cb;
	ctx->cbData = _cbData;

	return MSG_NO_ERROR;
}

MSG_READ_ERROR readBinaryMessage(struct COMM_DESC * _desc, char * _msg , u16 _size , CALLBACK _cb, void* _cbData){
	MSG_READ_CTX * ctx = &_desc->msgRd_ctx;

	if(ctx->State != MSG_RD_IDLE) return MSG_RD_BUSY;

	ctx->source = &_desc->rcvBuff;
	ctx->sink = _msg;
	ctx->size = _size;
	
	ctx->State = MSG_CNT_READ;
	ctx->cb = _cb;
	ctx->cbData = _cbData;
	return MSG_NO_ERROR;
}

char readChar(struct COMM_DESC * _desc){
	if(_desc->msgRd_ctx.State != MSG_RD_IDLE){
		return (char)-1;
	}
	return bufferRead(&_desc->rcvBuff);
}


void sendMessage(struct COMM_DESC * _desc, char * _msg, u16 _size , CALLBACK * _cb, void* _cbData){
//priprava na zasilani dlouhych zprav
}

COMM_ERROR sendShortMessage(struct COMM_DESC * _desc, char * _msg, u16 _size){
//zprava se ulozi do mistniho bufferu (neni-li zamcen)
	u16 cnt = 0;
	if(_desc->bLock){
		return CM_SND_LOCKED;
	}
	while (cnt < _size){
		bufferStore(&_desc->sndBuff, *_msg);
		cnt++;
		_msg++;
	}
	return CM_NO_ERROR;
}

COMM_ERROR sendShortMessage_Lock(struct COMM_DESC * _desc, char * _msg, u16 _size, BUFFER_KEY _key){
//zprava se ulozi do mistniho bufferu (neni-li zamcen)
	u16 cnt = 0;
	if(_desc->bLock != 0){
		if(_desc->bLock != _key){
			return CM_SND_LOCKED;		 
		}
	}
	while (cnt < _size){
		bufferStore(&_desc->sndBuff, *_msg);
		cnt++;
		_msg++;
	}
	return CM_NO_ERROR;
}


bool bufferStore(struct BUFFER * _buff, char _ch){

	if(_buff->Status == BUFF_FULL){
		return FALSE;
	}

	if(_buff->ptr_write == _buff->ptr_end){
		_buff->ptr_write = _buff->buffer;
	}else{
		_buff->ptr_write++;
	}
	
	*_buff->ptr_write = _ch;

	if( (_buff->ptr_write + 1) == _buff->ptr_read){	   //na dalsi zapis jiz neni prostor
		_buff->Status = BUFF_FULL;
		return TRUE;
	}

	if ((_buff->ptr_write == _buff->ptr_end) && (_buff->ptr_read == _buff->buffer)){
		_buff->Status = BUFF_FULL;
		return TRUE;	
	}

	_buff->Status = BUFF_RDY;
	return TRUE;
}



char bufferRead(struct BUFFER * _buff){

	if(_buff->Status == BUFF_FREE){
		return (char)-1;
	}

	if(_buff->ptr_read == _buff->ptr_end){
		_buff->ptr_read = _buff->buffer;
	}else{
		_buff->ptr_read++;
	}

	if( _buff->ptr_write == _buff->ptr_read){	   //priste neni co cist
		_buff->Status = BUFF_FREE;
		return *_buff->ptr_read;;
	}

	_buff->Status = BUFF_RDY;
	return *_buff->ptr_read;;
}


void init_buffer(struct BUFFER * _buff){
	_buff->ptr_write = _buff->buffer;
	_buff->ptr_read = _buff->buffer;
	_buff->ptr_end = &_buff->buffer[BUFFER_SIZE-1];	
}

void init_Comm(struct COMM_DESC * _desc){

	init_buffer(&_desc->sndBuff);
	init_buffer(&_desc->rcvBuff);

	_desc->Tick = &COMM_Tick;
	_desc->putChar = &putChar;
	_desc->getChar = &getChar;

	_desc->Error = CM_NO_ERROR;
	_desc->bLock = 0;
}
