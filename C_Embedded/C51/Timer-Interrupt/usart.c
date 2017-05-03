#include "usart.h"
#include <reg52.h>

void T0_Init()
{
	IE=0x82;						//中断允许寄存器
	TMOD=0x01;						//中断模式寄存器
//	TCON=0x10;					   	//中断空控制寄存器
	TH0=(65536-50000)/256;
	TL0=(65536-50000)%256;
}