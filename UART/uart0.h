#ifndef UART0_H

#define UART0_H


#include "types.h"

/* Function Declarations */

void UART0_Init(void);        // Function prototype for UART0 initialization

void UART0_TxChar(unsigned char); // Function prototype for transmitting one byte

unsigned char UART0_RxChar(void); // Function prototype for receiving one byte

void UART0_TxString(unsigned char *); // Function prototype for transmitting one string

void UART0_TxUInt(u32);

void UART0_TxSInt(s32);

void UART0_TxFloat(f32,u32);

void UART0_RxString(unsigned char *,unsigned int);
void hexUART_tx(u32 );

#endif
