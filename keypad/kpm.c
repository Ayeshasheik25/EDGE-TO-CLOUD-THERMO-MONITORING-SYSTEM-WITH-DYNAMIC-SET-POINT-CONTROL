#include"types.h"
#include<LPC21XX.h>
#include"defines.h"
#include"kpm_defines.h"
#include"lcd_defines.h"
#include"lcd.h"
#include"lcd1.h"
#include"delay.h"
/*
u32 kpmLUT[4][4]=
{
{1,2,3,4},
{5,6,7,8},
{9,10,11,12},
{13,14,15,16}
};
*/
u8 kpmLUT[4][4]=
{
{'7','8','9','/'},
{'4','5','6','X'},
{'1','2','3','-'},
{'C','0','=','+'}
};
void Init_KPM(void)
{
//cfg row lines as gpio output lines&
//ground all rows
WRITENIBBLE(IODIR1,ROWs_4,15);
//cfg col lines as gpio input(default anyway);
}
u32 ColScan(void)
{
return (READNIBBLE(IOPIN1,COLs_4)<15)?0:1;
}
u32 RowCheck(void)
{
u32 r;
for(r=0;r<=3;r++)
{
//ground one row at aq time, starting from 0th row
WRITENIBBLE(IOPIN1,ROWs_4,~(1<<r));
//check if key was pressed in that row
if(ColScan()==0)
{
break;
}
}
//re-initiaze all rows to ground
WRITENIBBLE(IOPIN1,ROWs_4,0);
return r;
}
u32 ColCheck(void)
{
u32 c;
for(c=0;c<=3;c++)
{
if(READBIT(IOPIN1,COL0+c)==0)
{
break;
}
}
return c;
}
u32 KeyScan(void)
{
u32 r,c,keyV;
//wait to detect any key press
while(ColScan());
//if key press detected
//identify row in which key was pressed
r=RowCheck();
//identify row in which was pressed
c=ColCheck();
//map key value to pressed key
keyV=kpmLUT[r][c];
return keyV;
}
void ReadNum( u32 *num,u8 *key)
{
*num=0;
while(1)
{
*key=KeyScan();
delay_ms(1000);
if(*key>='0'&& *key<='9')
{
*num=(*num*10)+(*key-48);
Write_CMD_LCD(GOTO_LINE2_POS0);
Write_int_LCD(*num);
while(ColScan()==0);
}
else if(*key=='C')
{
while(ColScan()==0);
if(*num>0)
{
*num=*num/10;
Write_CMD_LCD(0x10);
Write_DAT_LCD(' ');
Write_CMD_LCD(0x10);
}
}
else if(*key=='='||*key=='+'||*key=='-'||*key=='/'||*key=='X')
{
break;
}
else
{
Write_DAT_LCD(' ');
Write_DAT_LCD(*key);
while(ColScan()==0);
break;
}
}
}
