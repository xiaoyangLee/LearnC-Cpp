#include <REG51.H>
#include <intrins.h>
#define uint unsigned int
#define uchar unsigned char
sbit  DS1302_CLK = P1^7;              //ʵʱʱ��ʱ�������� 
sbit  DS1302_IO  = P1^6;              //ʵʱʱ������������ 
sbit  DS1302_RST = P1^5;              //ʵʱʱ�Ӹ�λ������
sbit  wireless_1 = P3^0;
sbit  wireless_2 = P3^1;
sbit  wireless_3 = P3^2;
sbit  wireless_4 = P3^3;
sbit  ACC0 = ACC^0;
sbit  ACC7 = ACC^7;
char hide_sec,hide_min,hide_hour,hide_day,hide_week,hide_month,hide_year;  //��,��,ʱ����,��,��λ���ļ���
sbit Set = P2^0;       //ģʽ�л���
sbit Up = P2^1;        //�ӷ���ť
sbit Down = P2^2;      //������ť
sbit out = P2^3;       //������������ģʽ��ť
sbit DQ = P1^0;        //�¶ȴ�������IO��
char done,count,temp,flag,up_flag,down_flag;
uchar temp_value;      //�¶�ֵ
uchar TempBuffer[5],week_value[2];


void show_time();   //Һ����ʾ����


sbit LcdRs		= P2^5;
sbit LcdRw		= P2^6;
sbit LcdEn  	= P2^7;
sfr  DBPort 	= 0x80;		//P0=0x80,P1=0x90,P2=0xA0,P3=0xB0.���ݶ˿�

//�ڲ��ȴ�����
unsigned char LCD_Wait(void)
{
	LcdRs=0;
	LcdRw=1;	_nop_();
	LcdEn=1;	_nop_();
						 
	LcdEn=0;
	return DBPort;		
}
//��LCDд�����������
#define LCD_COMMAND			0      // Command
#define LCD_DATA			1      // Data
#define LCD_CLEAR_SCREEN	0x01      // ����
#define LCD_HOMING  		0x02      // ��귵��ԭ��
void LCD_Write(bit style, unsigned char input)
{
	LcdEn=0;
	LcdRs=style;
	LcdRw=0;		_nop_();
	DBPort=input;	_nop_();//ע��˳��
	LcdEn=1;		_nop_();//ע��˳��
	LcdEn=0;		_nop_();
	LCD_Wait();	
}

//������ʾģʽ
#define LCD_SHOW			0x04    //��ʾ��
#define LCD_HIDE			0x00    //��ʾ��	  

#define LCD_CURSOR			0x02 	//��ʾ���
#define LCD_NO_CURSOR		0x00    //�޹��		     

#define LCD_FLASH			0x01    //�������
#define LCD_NO_FLASH		0x00    //��겻����

void LCD_SetDisplay(unsigned char DisplayMode)
{
	LCD_Write(LCD_COMMAND, 0x08|DisplayMode);	
}

//��������ģʽ
#define LCD_AC_UP			0x02
#define LCD_AC_DOWN			0x00      // default

#define LCD_MOVE			0x01      // �����ƽ��
#define LCD_NO_MOVE			0x00      //default

void LCD_SetInput(unsigned char InputMode)
{
	LCD_Write(LCD_COMMAND, 0x04|InputMode);
}

//��ʼ��LCD
void LCD_Initial()
{
	LcdEn=0;
	LCD_Write(LCD_COMMAND,0x38);           //8λ���ݶ˿�,2����ʾ,5*7����
	LCD_Write(LCD_COMMAND,0x38);
	LCD_SetDisplay(LCD_SHOW|LCD_NO_CURSOR);    //������ʾ, �޹��
	LCD_Write(LCD_COMMAND,LCD_CLEAR_SCREEN);   //����
	LCD_SetInput(LCD_AC_UP|LCD_NO_MOVE);       //AC����, ���治��
}

//Һ���ַ������λ��
void GotoXY(unsigned char x, unsigned char y)
{
	if(y==0)
		LCD_Write(LCD_COMMAND,0x80|x);
	if(y==1)
		LCD_Write(LCD_COMMAND,0x80|(x-0x40));
}

//���ַ������Һ����ʾ
void Print(unsigned char *str)
{
	while(*str!='\0')
	{
		LCD_Write(LCD_DATA,*str);
		str++;
	}
}


typedef struct __SYSTEMTIME__
{
	unsigned char Second;
	unsigned char Minute;
	unsigned char Hour;
	unsigned char Week;
	unsigned char Day;
	unsigned char Month;
	unsigned char  Year;
	unsigned char DateString[11];
	unsigned char TimeString[9];
}SYSTEMTIME;	//�����ʱ������
SYSTEMTIME CurrentTime;


#define AM(X)	X
#define PM(X)	(X+12)            	  // ת��24Сʱ��
#define DS1302_SECOND	0x80          //ʱ��оƬ�ļĴ���λ��,���ʱ��
#define DS1302_MINUTE	0x82
#define DS1302_HOUR		0x84 
#define DS1302_WEEK		0x8A
#define DS1302_DAY		0x86
#define DS1302_MONTH	0x88
#define DS1302_YEAR		0x8C 

void DS1302InputByte(unsigned char d) 	//ʵʱʱ��д��һ�ֽ�(�ڲ�����)
{ 
    unsigned char i;
    ACC = d;
    for(i=8; i>0; i--)
    {
        DS1302_IO = ACC0;           	//�൱�ڻ���е� RRC
        DS1302_CLK = 1;
        DS1302_CLK = 0;
        ACC = ACC >> 1; 
    } 
}

unsigned char DS1302OutputByte(void) 	//ʵʱʱ�Ӷ�ȡһ�ֽ�(�ڲ�����)
{ 
    unsigned char i;
    for(i=8; i>0; i--)
    {
        ACC = ACC >>1;         			//�൱�ڻ���е� RRC 
        ACC7 = DS1302_IO;
        DS1302_CLK = 1;
        DS1302_CLK = 0;
    } 
    return(ACC); 
}

void Write1302(unsigned char ucAddr, unsigned char ucDa)	//ucAddr: DS1302��ַ, ucData: Ҫд������
{
    DS1302_RST = 0;
    DS1302_CLK = 0;
    DS1302_RST = 1;
    DS1302InputByte(ucAddr);       	// ��ַ������ 
    DS1302InputByte(ucDa);       	// д1Byte����
    DS1302_CLK = 1;
    DS1302_RST = 0;
} 

unsigned char Read1302(unsigned char ucAddr)	//��ȡDS1302ĳ��ַ������
{
    unsigned char ucData;
    DS1302_RST = 0;
    DS1302_CLK = 0;
    DS1302_RST = 1;
    DS1302InputByte(ucAddr|0x01);        // ��ַ������ 
    ucData = DS1302OutputByte();         // ��1Byte����
    DS1302_CLK = 1;
    DS1302_RST = 0;
    return(ucData);
}



void DS1302_GetTime(SYSTEMTIME *Time)  //��ȡʱ��оƬ��ʱ�����ݵ��Զ���Ľṹ������
{
	unsigned char ReadValue;
	ReadValue = Read1302(DS1302_SECOND);
	Time->Second = ((ReadValue&0x70)>>4)*10 + (ReadValue&0x0F);
	ReadValue = Read1302(DS1302_MINUTE);
	Time->Minute = ((ReadValue&0x70)>>4)*10 + (ReadValue&0x0F);
	ReadValue = Read1302(DS1302_HOUR);
	Time->Hour = ((ReadValue&0x70)>>4)*10 + (ReadValue&0x0F);
	ReadValue = Read1302(DS1302_DAY);
	Time->Day = ((ReadValue&0x70)>>4)*10 + (ReadValue&0x0F);	
	ReadValue = Read1302(DS1302_WEEK);
	Time->Week = ((ReadValue&0x70)>>4)*10 + (ReadValue&0x0F);
	ReadValue = Read1302(DS1302_MONTH);
	Time->Month = ((ReadValue&0x70)>>4)*10 + (ReadValue&0x0F);
	ReadValue = Read1302(DS1302_YEAR);
	Time->Year = ((ReadValue&0x70)>>4)*10 + (ReadValue&0x0F);	
}

void DateToStr(SYSTEMTIME *Time)    //��ʱ����,��,��,����������DateString[]
{   if(hide_year<2)                 //if,else��䶼���ж�λ��˸
    {                               
	  Time->DateString[0] = '2';
	  Time->DateString[1] = '0';	 
	  Time->DateString[2] = Time->Year/10 + '0';
	  Time->DateString[3] = Time->Year%10 + '0';
	}
	  else
	    { 
	      Time->DateString[0] = ' ';
	      Time->DateString[1] = ' ';		 
	      Time->DateString[2] = ' ';
	      Time->DateString[3] = ' ';
		}
	Time->DateString[4] = '-';
	if(hide_month<2)
	{
	  Time->DateString[5] = Time->Month/10 + '0';
	  Time->DateString[6] = Time->Month%10 + '0';
	}
	  else
	  {
	    Time->DateString[5] = ' ';
	    Time->DateString[6] = ' ';
	  }
	Time->DateString[7] = '-';
	if(hide_day<2)
	{
	  Time->DateString[8] = Time->Day/10 + '0';
	  Time->DateString[9] = Time->Day%10 + '0';
	}
	  else
	  {
	    Time->DateString[8] = ' ';
	    Time->DateString[9] = ' ';	    
	  }
	if(hide_week<2)
	{
	  week_value[0] = Time->Week%10 + '0';  //���ڵ���������ŵ� week_value[]����
	}
	  else
	  {
	    week_value[0] = ' ';
	  }
	  week_value[1] = '\0';

	Time->DateString[10] = '\0'; //�ַ���ĩβ�� '\0' ,�жϽ����ַ�
}

void TimeToStr(SYSTEMTIME *Time)  //��ʱ,��,������ת����Һ����ʾ�ַ��ŵ����� TimeString[];
{   if(hide_hour<2)
    {
	  Time->TimeString[0] = Time->Hour/10 + '0';
	  Time->TimeString[1] = Time->Hour%10 + '0';
	}
	  else
	    {
	      Time->TimeString[0] = ' ';
	      Time->TimeString[1] = ' ';
		}
	Time->TimeString[2] = ':';
    if(hide_min<2)
	{
	  Time->TimeString[3] = Time->Minute/10 + '0';
	  Time->TimeString[4] = Time->Minute%10 + '0';
	}
	  else
	    {
	      Time->TimeString[3] = ' ';
	      Time->TimeString[4] = ' ';
   	    }
	Time->TimeString[5] = ':';
    if(hide_sec<2)
    {
	  Time->TimeString[6] = Time->Second/10 + '0';
	  Time->TimeString[7] = Time->Second%10 + '0';
    }
      else
       {
         Time->TimeString[6] = ' ';
	     Time->TimeString[7] = ' ';
       }
	Time->DateString[8] = '\0';
}


void Initial_DS1302(void)   //ʱ��оƬ��ʼ��
{   
	unsigned char Second=Read1302(DS1302_SECOND);
	if(Second&0x80)	      //�ж�ʱ��оƬ�Ƿ�ر�	  
    {
	Write1302(0x8e,0x00); //д������
	Write1302(0x8c,0x07); //����д���ʼ��ʱ�� ����:07/07/25.����: 3. ʱ��: 23:59:55
	Write1302(0x88,0x07);
	Write1302(0x86,0x25);
	Write1302(0x8a,0x07);
	Write1302(0x84,0x23);
	Write1302(0x82,0x59);
	Write1302(0x80,0x55);
	Write1302(0x8e,0x80); //��ֹд��
	}

}


void delay_18B20(unsigned int i)
{
	while(i--);
}

void Init_DS18B20(void) 
{
	 unsigned char x=0;
	 DQ = 1;          //DQ��λ
	 delay_18B20(8);  //������ʱ
	 DQ = 0;          //��Ƭ����DQ����
	 delay_18B20(80); //��ȷ��ʱ ���� 480us
	 DQ = 1;          //��������
	 delay_18B20(14);
	 x=DQ;            //������ʱ�� ���x=0���ʼ���ɹ� x=1���ʼ��ʧ��
	 delay_18B20(20);
}



unsigned char ReadOneChar(void)
{
	uchar i=0;
	uchar dat = 0;
	for (i=8;i>0;i--)
	 {
		  DQ = 0; // �������ź�
		  dat>>=1;
		  DQ = 1; // �������ź�
		  if(DQ)
		  dat|=0x80;
		  delay_18B20(4);
	 }
 	return(dat);
}



void WriteOneChar(uchar dat)
{
 	unsigned char i=0;
 	for (i=8; i>0; i--)
 	{
  		DQ = 0;
 		DQ = dat&0x01;
    	delay_18B20(5);
 		DQ = 1;
    	dat>>=1;
 }
}



void ReadTemp(void)
{
	unsigned char a=0;
	unsigned char b=0;
	unsigned char t=0;

	Init_DS18B20();
	WriteOneChar(0xCC);    	// ����������кŵĲ���
	WriteOneChar(0x44); 	// �����¶�ת��

	delay_18B20(100);       // this message is wery important

	Init_DS18B20();
	WriteOneChar(0xCC); 	//����������кŵĲ���
	WriteOneChar(0xBE); 	//��ȡ�¶ȼĴ����ȣ����ɶ�9���Ĵ����� ǰ���������¶�

	delay_18B20(100);

	a=ReadOneChar();    	//��ȡ�¶�ֵ��λ
	b=ReadOneChar();   		//��ȡ�¶�ֵ��λ
	temp_value=b<<4;
	temp_value+=(a&0xf0)>>4;               
}
void temp_to_str()   //�¶�����ת����Һ���ַ���ʾ
{
  TempBuffer[0]=temp_value/10+'0';  //ʮλ
  TempBuffer[1]=temp_value%10+'0';  //��λ
  TempBuffer[2]=' ';   //�¶ȷ���
  TempBuffer[3]='C';
  TempBuffer[4]='\0';
}
void Delay1ms(unsigned int count)
{
	unsigned int i,j;
	for(i=0;i<count;i++)
	for(j=0;j<120;j++);
}

/*��ʱ�ӳ���*/
void mdelay(uint delay)
{	uint i;
 	for(;delay>0;delay--)
   		{for(i=0;i<62;i++) //1ms��ʱ.
       		{;}
   		}
}


void outkey()                    //��������ģʽ,����Ĭ����ʾ
{ uchar Second;
  if(out==0||wireless_1==1)         
  { mdelay(8); 
	count=0;
	hide_sec=0,hide_min=0,hide_hour=0,hide_day=0,hide_week=0,hide_month=0,hide_year=0;
	Second=Read1302(DS1302_SECOND);
    Write1302(0x8e,0x00); //д������
	Write1302(0x80,Second&0x7f);
	Write1302(0x8E,0x80);          //��ֹд��
	done=0;           
	while(out==0);
	while(wireless_1==1);
  }
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Upkey()//���򰴼�
{	   
		Up=1;
		if(Up==0||wireless_2==1)
		          {
				   mdelay(8);
				       switch(count)
				          {case 1:
                                  temp=Read1302(DS1302_SECOND);  //��ȡ����
								  temp=temp+1;  //������1
                                  up_flag=1;    //���ݵ�������±�־
								  if((temp&0x7f)>0x59)   //����59��,����
                                  temp=0;								  
								  break;
				           case 2:
                                  temp=Read1302(DS1302_MINUTE);  //��ȡ����
								  temp=temp+1;  //������1
                                  up_flag=1;
								  if(temp>0x59)          //����59��,����
								  temp=0;
								  break;
				           case 3:
                                  temp=Read1302(DS1302_HOUR);  //��ȡСʱ��
								  temp=temp+1;  //Сʱ����1
                                  up_flag=1;
								  if(temp>0x23)   //����23Сʱ,����
								  temp=0;
								  break;
				           case 4:
                                  temp=Read1302(DS1302_WEEK);  //��ȡ������
								  temp=temp+1;  //��������1
                                  up_flag=1;
								  if(temp>0x7)  
								  temp=1;
								  break;
				           case 5:
                                  temp=Read1302(DS1302_DAY);  //��ȡ����
								  temp=temp+1;  //������1
                                  up_flag=1;
								  if(temp>0x31)
								  temp=1;
								  break;
				           case 6:
                                  temp=Read1302(DS1302_MONTH);  //��ȡ����
								  temp=temp+1;  //������1
                                  up_flag=1;
								  if(temp>0x12)
								  temp=1;
								  break;
				           case 7:
                                  temp=Read1302(DS1302_YEAR);  //��ȡ����
								  temp=temp+1;  //������1
                                  up_flag=1;
								  if(temp>0x85)
								  temp=0;
								  break;
					       default:break;
				          }
					  
				   while(Up==0);
	               while(wireless_2==1);
				  }
}

void Downkey()//���򰴼�
{	    
		Down=1;
	    if(Down==0||wireless_3==1)
		          {
				   mdelay(8);
				     switch(count)
				          {case 1:
                                  temp=Read1302(DS1302_SECOND);  //��ȡ����
								  temp=temp-1;						    //������1
                                  down_flag=1;       //���ݵ�������±�־
								  if(temp==0x7f)     //С��0��,����59��
								  temp=0x59;
								  break;
				           case 2:
                                  temp=Read1302(DS1302_MINUTE);  //��ȡ����
								  temp=temp-1;  //������1
                                  down_flag=1;
								  if(temp==-1)
								  temp=0x59;      //С��0��,����59��
								  break;
				           case 3:
                                  temp=Read1302(DS1302_HOUR);  //��ȡСʱ��
								  temp=temp-1;  //Сʱ����1
                                  down_flag=1;
								  if(temp==-1)
								  temp=0x23;
								  break;
				           case 4:
                                  temp=Read1302(DS1302_WEEK);  //��ȡ������
								  temp=temp-1;  //��������1
                                  down_flag=1;
								  if(temp==0)
								  temp=0x7;;
								  break;
				           case 5:
                                  temp=Read1302(DS1302_DAY);  //��ȡ����
								  temp=temp-1;  //������1
                                  down_flag=1;
								  if(temp==0)
								  temp=31;
								  break;
				           case 6:
                                  temp=Read1302(DS1302_MONTH);  //��ȡ����
								  temp=temp-1;  //������1
                                  down_flag=1;
								  if(temp==0)
								  temp=12;
								  break;
				           case 7:
                                  temp=Read1302(DS1302_YEAR);  //��ȡ����
								  temp=temp-1;  //������1
                                  down_flag=1;
								  if(temp==-1)
								  temp=0x85;
								  break;
					      default:break;
				         }
					 
				   while(Down==0);
				   while(wireless_3==1);
				  }
}

void Setkey()//ģʽѡ�񰴼�
{
		Set=1;
		if(Set==0||wireless_4==1)
	    {
           mdelay(8);
           count=count+1;	 //Setkey��һ��,count�ͼ�1
		   done=1;			 //�������ģʽ
           while(Set==0);
           while(wireless_4==1);
		 }

}

void keydone()//��������ִ��
{        uchar Second;
		 if(flag==0)    //�ر�ʱ��,ֹͣ��ʱ
         { Write1302(0x8e,0x00); //д������
           temp=Read1302(0x80);
           Write1302(0x80,temp|0x80);
	       Write1302(0x8e,0x80); //��ֹд��
           flag=1;
         }
         Setkey();				            //ɨ��ģʽ�л�����
		 switch(count)
		 {case 1:do					        //count=1,������
		          {
                   outkey();			   //ɨ��������ť
				   Upkey();                //ɨ��Ӱ�ť
				   Downkey();              //ɨ�����ť
				   if(up_flag==1||down_flag==1)  //���ݸ��£�����д���µ�����
				   {
				   Write1302(0x8e,0x00); //д������
				   Write1302(0x80,temp|0x80); //д���µ�����
				   Write1302(0x8e,0x80); //��ֹд��
				   up_flag=0;
				   down_flag=0;
				   }

				   hide_sec++;          //λ������
				   if(hide_sec>3)
				     hide_sec=0;
                   show_time();         //Һ����ʾ����
				  }while(count==2);break;  
		  case 2:do					        //count=2,������
		          {
				   hide_sec=0;
				   outkey();
				   Upkey();
				   Downkey();
				   if(temp>0x60)
				     temp=0;
				   if(up_flag==1||down_flag==1)
				   {
				   Write1302(0x8e,0x00); //д������
				   Write1302(0x82,temp); //д���µķ���
				   Write1302(0x8e,0x80); //��ֹд��
				   up_flag=0;
				   down_flag=0;
				   }
				   hide_min++;
				   if(hide_min>3)
				     hide_min=0;
                   show_time();
				  }while(count==3);break;
		  case 3:do					        //count=3,����Сʱ
		          {
                   hide_min=0; 
				   outkey();
				   Upkey();
				   Downkey();
				   if(up_flag==1||down_flag==1)
				   {
				   Write1302(0x8e,0x00); //д������
				   Write1302(0x84,temp); //д���µ�Сʱ��
				   Write1302(0x8e,0x80); //��ֹд��
				   up_flag=0;
				   down_flag=0;
				   }
				   hide_hour++;
				   if(hide_hour>3)
				     hide_hour=0;
                   show_time();
				  }while(count==4);break;
		  case 4:do					        //count=4,��������
		          {
                   hide_hour=0; 
				   outkey();
				   Upkey();
				   Downkey();
				   if(up_flag==1||down_flag==1)
				   {
				   Write1302(0x8e,0x00); //д������
				   Write1302(0x8a,temp); //д���µ�������
				   Write1302(0x8e,0x80); //��ֹд��
				   up_flag=0;
				   down_flag=0;
				   }
				   hide_week++;
				   if(hide_week>3)
				     hide_week=0;
                   show_time();
				  }while(count==5);break;
		  case 5:do					        //count=5,������
		          {
				   hide_week=0; 
				   outkey();
				   Upkey();
				   Downkey();
				   if(up_flag==1||down_flag==1)
				   {
				   Write1302(0x8e,0x00); //д������
				   Write1302(0x86,temp); //д���µ�����
				   Write1302(0x8e,0x80); //��ֹд��
				   up_flag=0;
				   down_flag=0;
				   }
				   hide_day++;
				   if(hide_day>3)
				     hide_day=0;
                   show_time();
				  }while(count==6);break;
		  case 6:do					        //count=6,������
		          {
                   hide_day=0; 
				   outkey();
				   Upkey();
				   Downkey();
				   if(up_flag==1||down_flag==1)
				   {
				   Write1302(0x8e,0x00); //д������
				   Write1302(0x88,temp); //д���µ�����
				   Write1302(0x8e,0x80); //��ֹд��
				   up_flag=0;
				   down_flag=0;
				   }
				   hide_month++;
				   if(hide_month>3)
				     hide_month=0;
                   show_time();
				  }while(count==7);break;
		  case 7:do					        //count=7,������
		          {
                   hide_month=0; 
				   outkey();
				   Upkey();
				   Downkey();
				   if(up_flag==1||down_flag==1)
				   {
				   Write1302(0x8e,0x00); //д������
				   Write1302(0x8c,temp); //д���µ�����
				   Write1302(0x8e,0x80); //��ֹд��
				   up_flag=0;
				   down_flag=0;
				   }
				   hide_year++;
				   if(hide_year>3)
				     hide_year=0;
                   show_time();
				  }while(count==8);break;
		  case 8: count=0;hide_year=0;  //count8, ��������ģʽ,����Ĭ����ʾ״̬
	              Second=Read1302(DS1302_SECOND);
                  Write1302(0x8e,0x00); //д������
	              Write1302(0x80,Second&0x7f);
	              Write1302(0x8E,0x80);          //��ֹд��
				  done=0;
		  break; //count=7,�����ж�,��־λ��0���˳�
		  default:break;

		 }

}


void show_time()   //Һ����ʾ����
{
  DS1302_GetTime(&CurrentTime);  //��ȡʱ��оƬ��ʱ������
  TimeToStr(&CurrentTime);       //ʱ������ת��Һ���ַ�
  DateToStr(&CurrentTime);       //��������ת��Һ���ַ�
  ReadTemp();                    //�����¶Ȳɼ�����
  temp_to_str();                 //�¶�����ת����Һ���ַ�
  GotoXY(12,1);                  //Һ���ַ���ʾλ��
  Print(TempBuffer);             //��ʾ�¶�
  GotoXY(0,1);
  Print(CurrentTime.TimeString); //��ʾʱ��
  GotoXY(0,0);
  Print(CurrentTime.DateString); //��ʾ����
  GotoXY(15,0);
  Print(week_value);             //��ʾ����
  GotoXY(11,0);
  Print("zhou");	//��Һ������ʾ ��ĸ week
  Delay1ms(400);                 //ɨ����ʱ
}



main()
{
    flag=1;           //ʱ��ֹͣ��־
	LCD_Initial();    //Һ����ʼ��
	Init_DS18B20( ) ;      //DS18B20��ʼ��
	Initial_DS1302(); //ʱ��оƬ��ʼ��
	up_flag=0;
	down_flag=0;
	done=0;           //����Ĭ��Һ����ʾ
	wireless_1=0;
	wireless_2=0;
	wireless_3=0;
	wireless_4=0;
	while(1)
	{   
        while(done==1)
          keydone();    //�������ģʽ
		while(done==0)
 	    {  
            show_time();                //Һ����ʾ����
            flag=0;                  
  		    Setkey();				 //ɨ������ܼ�
 		}
	}
}




