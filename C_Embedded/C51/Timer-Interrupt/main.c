#include <reg52.h>

typedef unsigned char uchar;
typedef unsigned int uint;

sbit P20=P2^0;
sbit P21=P2^1;

uchar count = 0;

uchar led_code[]={
0x3f,0x06,0x5b,0x4f,
0x66,0x6d,0x7d,0x07,
0x7f,0x6f};

void Delay(uchar z)
{
	uchar a,b;
	for(a=z;a>0;a--)
	for(b=110;b>0;b--);	
}

void Int0_config()
{
	IT0=1;		  //set INT0 interrupt type(1: Falling 0: Low level)
	EX0=1;		  //enable INT0 inturrupt
	EA=1;		  //open globel inturrupt seitch
}

void led_show()
{
	uint x,y;
	x = count/10;	 //十位
	y = count%10;	 //个位

	if(count <10)
	{
		P20 = 0;
		P0 = led_code[y];
	}
	else
	{
		P20 = 0;
		P0 = led_code[y];
		Delay(10);
		P20=1;

		P21 = 0;
		P0 = led_code[x];
		Delay(10);
		P21=1;
	}
}

void main()
{
	Int0_config();
	while(1)
	{
		led_show();
	}
}

void int0() interrupt 0
{
	count++;
	if(count == 100)
	{
		count = 0;
	}
}