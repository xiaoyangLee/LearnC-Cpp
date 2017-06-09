#include "reg51.h" 
#define uint unsigned int  
#define uchar unsigned char  

void send(uchar state) 
{ 
SBUF=state; 
while(TI==0); 
TI=0; 
} 
void SCON_init(void) 
{ 
SCON=0x50; 
TMOD=0x20; 
PCON=0x00; 
TH1=0xfd; 
TL1=0xfd; 
TI=0;
TR1=1; 
} 
void main() 
{ 
 
P1=0xff; 
 
SCON_init(); 
 
while(1) 

{ 
send(P1); 
 
} 
} 
