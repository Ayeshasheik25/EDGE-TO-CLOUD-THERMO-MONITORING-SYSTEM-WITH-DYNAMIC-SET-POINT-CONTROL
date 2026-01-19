#include "types.h"
void delay_ms(u32 dlyMS)
{
  u32 j;
  for(;dlyMS>0;dlyMS--)
  {
   for(j=12000;j>0;j--);
  }
}
void delay_us(u32 dlyUS)
{
  u32 j;
  for(;dlyUS>0;dlyUS--)
  {
   for(j=12;j>0;j--);
  }
}
void delay_s(u32 dlyS)
{
  u32 j;
  for(;dlyS>0;dlyS--)
  {
   for(j=12000000;j>0;j--);
  }
}

