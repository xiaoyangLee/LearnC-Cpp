#ifndef _CALENDAR_H_
#define _CALENDAR_H_
sbit  DS1302_CLK = P1^7;              //实时时钟时钟线引脚 
sbit  DS1302_IO  = P1^6;              //实时时钟数据线引脚 
sbit  DS1302_RST = P1^5;              //实时时钟复位线引脚
sbit  wireless_1 = P3^0;
sbit  wireless_2 = P3^1;
sbit  wireless_3 = P3^2;
sbit  wireless_4 = P3^3;
sbit  ACC0 = ACC^0;
sbit  ACC7 = ACC^7;
extern char hide_sec,hide_min,hide_hour,hide_day,hide_week,hide_month,hide_year;  //秒,分,时到日,月,年位闪的计数
sbit Set = P2^0;       //模式切换键
sbit Up = P2^1;        //加法按钮
sbit Down = P2^2;      //减法按钮
sbit out = P2^3;       //立刻跳出调整模式按钮
sbit DQ = P1^0;        //温度传送数据IO口
extern char done,count,temp,flag,up_flag,down_flag;
extern unsigned char temp_value;      //温度值
extern unsigned char TempBuffer[5],week_value[2];


#endif