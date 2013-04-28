#ifndef _NVIC_BASIC_H
#define _NVIC_BASIC_H

/* Exported function prototypes */
void NVIC_IntEnable (u8 Channel, u8 Priority);
void NVIC_IntDisable (u8 Channel);
void NVIC_ClearPending (u8 Channel);

#endif
