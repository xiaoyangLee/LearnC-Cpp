#include <reg52.h>
/*
此程序用在STC12C5A60S2芯片上，用作控制两块点阵显示字符“DH”
*/
void delay(unsigned int count)
{
	int i=0;
	while(count--)
	for(i=120;i>0;i--);
}

int main()
{
	 while(1)
	 {
	 
	 	P1=0x00;
		P0=0x00;
		P2=0xFF;
		delay(10);
	
		P1=0x7E;
		P0=0x7E;
		P2=0xFD;
		delay(10);
	 
		P1=0x42;
		P0=0x10;
		P2=0xFB;
		delay(10);
	
		P1=0x24;
		P0=0x10;
		P2=0xF7;
		delay(10);

		P1=0x18;
		P0=0x7E;
		P2=0xEF;
		delay(10);

		P1=0x00;
		P0=0x00;
		P2=0xFF;
		delay(10);

		P1=0x00;
		P0=0x00;
		P2=0xFF;
		delay(10);

		P1=0x00;
		P0=0x00;
		P2=0xFF;
		delay(10);
	 }
}
