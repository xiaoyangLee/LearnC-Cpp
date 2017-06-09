#include "reg51.h" 
#define uint unsigned int  
#define uchar unsigned char  
uchar state; 
void receive() 
{ 
 
while(RI==0);
 
state=SBUF; 
 
RI=0; 
} 
 
void SCON_init(void) 
{ 
 
SCON=0x50; 
 
TMOD=0x20; 
 
PCON=0x00; 
 
TH1=0xfd; 
 
TL1=0xfd; 
 
RI=0; 
 
TR1=1; 
 
} 
 
void main() 
{ 
SCON_init(); 
 
while(1) 
{
receive(); 
P1=state;
} 
}
