#ifndef _NVIC_BASIC_H
#define _NVIC_BASIC_H

/* Exported function prototypes */
void NVIC_IntEnable (uint8_t Channel, uint8_t Priority);
void NVIC_IntDisable (uint8_t Channel);
void NVIC_ClearPending (uint8_t Channel);

#endif
