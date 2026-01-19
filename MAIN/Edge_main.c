#include<LPC214x.h>
#include"types.h"
#include"lcd1.h"
#include"kpm.h"
#include"delay.h"
#include"dht11.h"
#include"esp01.h"
#include"uart0.h"
#include"pin_connect_block.h"
#include"i2c.h"
#include"rtc1.h"
#include"i2c_eeprom.h"
#include"delay.h"
#include<stdio.h>
#include<stdlib.h>
#define BUZZER_PIN  (1<<10)
#define EEPROM_ADDR 0x001
#define EEPROM_SLAVE 0x50
#define RTC_UPLOAD_INTERVAL_MIN 3
volatile u8 alarm_active=0;
volatile u8 setpoint_update_req=0;
u8 prev_min=0;
u8 cur_min=0;
u8 data=0;
void display_values(u8 hum_int, u8 hum_dec, u8 temp_int ,u8 temp_dec, u32 setp)
{
Write_CMD_LCD(0x80);
Write_str_LCD("H: ");	 
Write_int_LCD(hum_int);
//Write_DAT_LCD('.');
//Write_int_LCD(hum_dec);
Write_str_LCD(" %RH ");;

Write_CMD_LCD(0xc0);
Write_str_LCD("T: ");
Write_int_LCD(temp_int);
//Write_DAT_LCD('.');
//Write_int_LCD(temp_dec);
Write_DAT_LCD(223);
Write_str_LCD("C ");

Write_CMD_LCD(0x89);
Write_str_LCD("S: ");
Write_int_LCD(setp);
}
void eint0_isr(void)__irq
{
alarm_active=0;
IOCLR0=BUZZER_PIN;
setpoint_update_req=1;
EXTINT |=(1<<0);
VICVectAddr=0;
}
void eint0_Init(void)
{
cfgPortPinFunc(0,14,2);
EXTMODE |=(1<<1);
EXTPOLAR &=~(1<<1);
EXTINT |=(1<<1);
VICVectAddr1=(u32)eint0_isr;
VICVectCntl1=0x20|15;
VICIntEnable |=(1<<15);
}

u32 Load_setpoint_from_EEPROM(void)
{
u8 sp= i2c_eeprom_read(EEPROM_SLAVE,EEPROM_ADDR);
/*if(sp==0xff ||sp==0x00)
{
i2c_eeprom_write(EEPROM_SLAVE,EEPROM_ADDR,30);
return 30;
}
else
{ */
return sp;
//}
}

int main()
{
u8 hum_int=0,hum_dec=0,temp_int=0,temp_dec=0,checksum=0;
u32 new_val=0;
u8 key;
u32 set_point=0;
char temp_str[6];
char hum_str[6];
char alert_str[6];
LCD_Init();
Init_KPM();
InitUART0();
init_i2c();
RTC_Init();
eint0_Init();
IODIR0|=BUZZER_PIN;
IOCLR0 =BUZZER_PIN;
esp01_connectAP();
set_point=Load_setpoint_from_EEPROM();

Write_CMD_LCD(0x01);
Write_str_LCD("System Ready");
delay_ms(1000);
while(1)
{
dht11_request();
dht11_response();
hum_int=dht11_data();
hum_dec=dht11_data();
temp_int=dht11_data();
temp_dec=dht11_data();
checksum=dht11_data();
if((hum_int+hum_dec+temp_int+temp_dec)!=checksum)
{
Write_CMD_LCD(0x01);
Write_str_LCD("checksum Error");
delay_ms(1500);
continue;
}
display_values(hum_int,hum_dec,temp_int,temp_dec,set_point);
sprintf(temp_str,"%u",temp_int);
sprintf(hum_str,"%u",hum_int);
if(data==0)
{
if(temp_int>=set_point)
{
data++;
alarm_active=1;
IOSET0=BUZZER_PIN;
delay_ms(300);
IOCLR0=BUZZER_PIN;
delay_ms(300);
sprintf(alert_str,"%u",temp_int);
esp01_sendToThingspeak(temp_str,hum_str,alert_str);
}
}
//else
//{
//sprintf(alert_str,"0");
//}
if(data<20)
{
data++;
}
else
{
data=0;
}
cur_min=RTC_GetMin();
if((u8)(cur_min -prev_min)>=RTC_UPLOAD_INTERVAL_MIN)
{
prev_min=cur_min;
esp01_sendToThingspeak(temp_str,hum_str,alert_str);
alarm_active=0;
}
if(setpoint_update_req==1)
{
setpoint_update_req=0;
Write_CMD_LCD(0x01);
Write_str_LCD("update set point:");
Write_CMD_LCD(0xc0);
new_val=0;
ReadNum(&new_val,&key);
if(new_val>0 && new_val<100)
{
set_point=new_val;
i2c_eeprom_write(EEPROM_SLAVE,EEPROM_ADDR,(u8)set_point);
Write_CMD_LCD(0x01);
Write_str_LCD("Updated");
Write_CMD_LCD(0x01);
Write_CMD_LCD(0xc0);
Write_str_LCD("SP:");
Write_int_LCD(set_point);
delay_ms(1200);
}
else
{
Write_CMD_LCD(0x01);
Write_str_LCD("invalid input");
delay_ms(800);
}
}
delay_ms(2000);
}
}



