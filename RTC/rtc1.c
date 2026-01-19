#include<LPC214X.h>

#define FOSC 12000000
#define CCLK 5*FOSC
#define PCLK CCLK/4
#define PREINT_VAL ((PCLK/32768)-1)
#define PREFRAC_VAL (PCLK-((PREINT_VAL+1)*32768))
void RTC_Init(void)
{
PREINT=PREINT_VAL;
PREFRAC=PREFRAC_VAL;
CCR=0x11;
}
unsigned char RTC_GetMin(void)
{
return MIN;
}