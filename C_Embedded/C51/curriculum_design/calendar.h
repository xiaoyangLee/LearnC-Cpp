#ifndef _CALENDAR_H_
#define _CALENDAR_H_
sbit  DS1302_CLK = P1^7;              //ʵʱʱ��ʱ�������� 
sbit  DS1302_IO  = P1^6;              //ʵʱʱ������������ 
sbit  DS1302_RST = P1^5;              //ʵʱʱ�Ӹ�λ������
sbit  wireless_1 = P3^0;
sbit  wireless_2 = P3^1;
sbit  wireless_3 = P3^2;
sbit  wireless_4 = P3^3;
sbit  ACC0 = ACC^0;
sbit  ACC7 = ACC^7;
extern char hide_sec,hide_min,hide_hour,hide_day,hide_week,hide_month,hide_year;  //��,��,ʱ����,��,��λ���ļ���
sbit Set = P2^0;       //ģʽ�л���
sbit Up = P2^1;        //�ӷ���ť
sbit Down = P2^2;      //������ť
sbit out = P2^3;       //������������ģʽ��ť
sbit DQ = P1^0;        //�¶ȴ�������IO��
extern char done,count,temp,flag,up_flag,down_flag;
extern unsigned char temp_value;      //�¶�ֵ
extern unsigned char TempBuffer[5],week_value[2];


#endif