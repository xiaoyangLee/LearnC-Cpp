#include "usart.h"
#include <reg52.h>

void T0_Init()
{
	IE=0x82;						//�ж�����Ĵ���
	TMOD=0x01;						//�ж�ģʽ�Ĵ���
//	TCON=0x10;					   	//�жϿտ��ƼĴ���
	TH0=(65536-50000)/256;
	TL0=(65536-50000)%256;
}