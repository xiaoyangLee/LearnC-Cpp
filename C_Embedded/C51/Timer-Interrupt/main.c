#include <reg52.h>
#include "usart.h"

uchar code Led_mod[]={0x38,0x3f,0x3e,0x73};
bit flag = 0;
uint count;
sbit P30 = P3^0;
sbit P31 = P3^1;

void Delay(uchar z)
{
	uchar a,b;
	for(a=z; a>0; a--)
	for(b=110; b>0; b--);
}

void Display_LO(uchar i)
{
	P30 = 0;
	P2 = Led_mod[i];
	Delay(10);
	P30 = 1;

	P31 = 0;
	P2 = Led_mod[i+1];
	Delay(10);
	P31 = 1;
}

void main()
{
	uint j;
	T0_Init();
	while(1)
	{
		Display_LO(j);
//		if(TF0 == 1)
		while(TF0)
		{
			TH0=(65536-50000)/256;
			TL0=(65536-50000)%256;
			count++;
			TF0 = 0;
		}
		if(count == 5)
		{
			count = 0;
			flag = ~flag;
			if(flag == 1)
			{
				j=2;
			}
			else
			{
				j=0;
			}
		}
	}
}