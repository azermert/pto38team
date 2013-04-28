#ifndef _MAIN_H
#define _MAIN_H 

void msgAdd(char* text);
void ethRead(char* read);
u16	StringToSend(void);
void SetPos(void);
void CarriageOnCenter(void);

extern int pozice;
extern s32 target_array[];
extern s32 *target;					//pointer to target value
extern s32 *target_array_end;		//end of array
extern s32 *last_target;			//pointer to last target in array
extern u8  Primary_tick_enable;

extern u16 AD_Vsense_OFFSET;

#endif
