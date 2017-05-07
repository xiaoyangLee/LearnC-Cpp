#include <REG51.H>
#include <intrins.h>
#define uint unsigned int
#define uchar unsigned char
sbit  DS1302_CLK = P1^7;              //实时时钟时钟线引脚 
sbit  DS1302_IO  = P1^6;              //实时时钟数据线引脚 
sbit  DS1302_RST = P1^5;              //实时时钟复位线引脚
sbit  wireless_1 = P3^0;
sbit  wireless_2 = P3^1;
sbit  wireless_3 = P3^2;
sbit  wireless_4 = P3^3;
sbit  ACC0 = ACC^0;
sbit  ACC7 = ACC^7;
char hide_sec,hide_min,hide_hour,hide_day,hide_week,hide_month,hide_year;  //秒,分,时到日,月,年位闪的计数
sbit Set = P2^0;       //模式切换键
sbit Up = P2^1;        //加法按钮
sbit Down = P2^2;      //减法按钮
sbit out = P2^3;       //立刻跳出调整模式按钮
sbit DQ = P1^0;        //温度传送数据IO口
char done,count,temp,flag,up_flag,down_flag;
uchar temp_value;      //温度值
uchar TempBuffer[5],week_value[2];


void show_time();   //液晶显示程序


sbit LcdRs		= P2^5;
sbit LcdRw		= P2^6;
sbit LcdEn  	= P2^7;
sfr  DBPort 	= 0x80;		//P0=0x80,P1=0x90,P2=0xA0,P3=0xB0.数据端口

//内部等待函数
unsigned char LCD_Wait(void)
{
	LcdRs=0;
	LcdRw=1;	_nop_();
	LcdEn=1;	_nop_();
						 
	LcdEn=0;
	return DBPort;		
}
//向LCD写入命令或数据
#define LCD_COMMAND			0      // Command
#define LCD_DATA			1      // Data
#define LCD_CLEAR_SCREEN	0x01      // 清屏
#define LCD_HOMING  		0x02      // 光标返回原点
void LCD_Write(bit style, unsigned char input)
{
	LcdEn=0;
	LcdRs=style;
	LcdRw=0;		_nop_();
	DBPort=input;	_nop_();//注意顺序
	LcdEn=1;		_nop_();//注意顺序
	LcdEn=0;		_nop_();
	LCD_Wait();	
}

//设置显示模式
#define LCD_SHOW			0x04    //显示开
#define LCD_HIDE			0x00    //显示关	  

#define LCD_CURSOR			0x02 	//显示光标
#define LCD_NO_CURSOR		0x00    //无光标		     

#define LCD_FLASH			0x01    //光标闪动
#define LCD_NO_FLASH		0x00    //光标不闪动

void LCD_SetDisplay(unsigned char DisplayMode)
{
	LCD_Write(LCD_COMMAND, 0x08|DisplayMode);	
}

//设置输入模式
#define LCD_AC_UP			0x02
#define LCD_AC_DOWN			0x00      // default

#define LCD_MOVE			0x01      // 画面可平移
#define LCD_NO_MOVE			0x00      //default

void LCD_SetInput(unsigned char InputMode)
{
	LCD_Write(LCD_COMMAND, 0x04|InputMode);
}

//初始化LCD
void LCD_Initial()
{
	LcdEn=0;
	LCD_Write(LCD_COMMAND,0x38);           //8位数据端口,2行显示,5*7点阵
	LCD_Write(LCD_COMMAND,0x38);
	LCD_SetDisplay(LCD_SHOW|LCD_NO_CURSOR);    //开启显示, 无光标
	LCD_Write(LCD_COMMAND,LCD_CLEAR_SCREEN);   //清屏
	LCD_SetInput(LCD_AC_UP|LCD_NO_MOVE);       //AC递增, 画面不动
}

//液晶字符输入的位置
void GotoXY(unsigned char x, unsigned char y)
{
	if(y==0)
		LCD_Write(LCD_COMMAND,0x80|x);
	if(y==1)
		LCD_Write(LCD_COMMAND,0x80|(x-0x40));
}

//将字符输出到液晶显示
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
}SYSTEMTIME;	//定义的时间类型
SYSTEMTIME CurrentTime;


#define AM(X)	X
#define PM(X)	(X+12)            	  // 转成24小时制
#define DS1302_SECOND	0x80          //时钟芯片的寄存器位置,存放时间
#define DS1302_MINUTE	0x82
#define DS1302_HOUR		0x84 
#define DS1302_WEEK		0x8A
#define DS1302_DAY		0x86
#define DS1302_MONTH	0x88
#define DS1302_YEAR		0x8C 

void DS1302InputByte(unsigned char d) 	//实时时钟写入一字节(内部函数)
{ 
    unsigned char i;
    ACC = d;
    for(i=8; i>0; i--)
    {
        DS1302_IO = ACC0;           	//相当于汇编中的 RRC
        DS1302_CLK = 1;
        DS1302_CLK = 0;
        ACC = ACC >> 1; 
    } 
}

unsigned char DS1302OutputByte(void) 	//实时时钟读取一字节(内部函数)
{ 
    unsigned char i;
    for(i=8; i>0; i--)
    {
        ACC = ACC >>1;         			//相当于汇编中的 RRC 
        ACC7 = DS1302_IO;
        DS1302_CLK = 1;
        DS1302_CLK = 0;
    } 
    return(ACC); 
}

void Write1302(unsigned char ucAddr, unsigned char ucDa)	//ucAddr: DS1302地址, ucData: 要写的数据
{
    DS1302_RST = 0;
    DS1302_CLK = 0;
    DS1302_RST = 1;
    DS1302InputByte(ucAddr);       	// 地址，命令 
    DS1302InputByte(ucDa);       	// 写1Byte数据
    DS1302_CLK = 1;
    DS1302_RST = 0;
} 

unsigned char Read1302(unsigned char ucAddr)	//读取DS1302某地址的数据
{
    unsigned char ucData;
    DS1302_RST = 0;
    DS1302_CLK = 0;
    DS1302_RST = 1;
    DS1302InputByte(ucAddr|0x01);        // 地址，命令 
    ucData = DS1302OutputByte();         // 读1Byte数据
    DS1302_CLK = 1;
    DS1302_RST = 0;
    return(ucData);
}



void DS1302_GetTime(SYSTEMTIME *Time)  //获取时钟芯片的时钟数据到自定义的结构型数组
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

void DateToStr(SYSTEMTIME *Time)    //将时间年,月,日,星期数组里DateString[]
{   if(hide_year<2)                 //if,else语句都是判断位闪烁
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
	  week_value[0] = Time->Week%10 + '0';  //星期的数据另外放到 week_value[]数组
	}
	  else
	  {
	    week_value[0] = ' ';
	  }
	  week_value[1] = '\0';

	Time->DateString[10] = '\0'; //字符串末尾加 '\0' ,判断结束字符
}

void TimeToStr(SYSTEMTIME *Time)  //将时,分,秒数据转换成液晶显示字符放到数组 TimeString[];
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


void Initial_DS1302(void)   //时钟芯片初始化
{   
	unsigned char Second=Read1302(DS1302_SECOND);
	if(Second&0x80)	      //判断时钟芯片是否关闭	  
    {
	Write1302(0x8e,0x00); //写入允许
	Write1302(0x8c,0x07); //以下写入初始化时间 日期:07/07/25.星期: 3. 时间: 23:59:55
	Write1302(0x88,0x07);
	Write1302(0x86,0x25);
	Write1302(0x8a,0x07);
	Write1302(0x84,0x23);
	Write1302(0x82,0x59);
	Write1302(0x80,0x55);
	Write1302(0x8e,0x80); //禁止写入
	}

}


void delay_18B20(unsigned int i)
{
	while(i--);
}

void Init_DS18B20(void) 
{
	 unsigned char x=0;
	 DQ = 1;          //DQ复位
	 delay_18B20(8);  //稍做延时
	 DQ = 0;          //单片机将DQ拉低
	 delay_18B20(80); //精确延时 大于 480us
	 DQ = 1;          //拉高总线
	 delay_18B20(14);
	 x=DQ;            //稍做延时后 如果x=0则初始化成功 x=1则初始化失败
	 delay_18B20(20);
}



unsigned char ReadOneChar(void)
{
	uchar i=0;
	uchar dat = 0;
	for (i=8;i>0;i--)
	 {
		  DQ = 0; // 给脉冲信号
		  dat>>=1;
		  DQ = 1; // 给脉冲信号
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
	WriteOneChar(0xCC);    	// 跳过读序号列号的操作
	WriteOneChar(0x44); 	// 启动温度转换

	delay_18B20(100);       // this message is wery important

	Init_DS18B20();
	WriteOneChar(0xCC); 	//跳过读序号列号的操作
	WriteOneChar(0xBE); 	//读取温度寄存器等（共可读9个寄存器） 前两个就是温度

	delay_18B20(100);

	a=ReadOneChar();    	//读取温度值低位
	b=ReadOneChar();   		//读取温度值高位
	temp_value=b<<4;
	temp_value+=(a&0xf0)>>4;               
}
void temp_to_str()   //温度数据转换成液晶字符显示
{
  TempBuffer[0]=temp_value/10+'0';  //十位
  TempBuffer[1]=temp_value%10+'0';  //个位
  TempBuffer[2]=' ';   //温度符号
  TempBuffer[3]='C';
  TempBuffer[4]='\0';
}
void Delay1ms(unsigned int count)
{
	unsigned int i,j;
	for(i=0;i<count;i++)
	for(j=0;j<120;j++);
}

/*延时子程序*/
void mdelay(uint delay)
{	uint i;
 	for(;delay>0;delay--)
   		{for(i=0;i<62;i++) //1ms延时.
       		{;}
   		}
}


void outkey()                    //跳出调整模式,返回默认显示
{ uchar Second;
  if(out==0||wireless_1==1)         
  { mdelay(8); 
	count=0;
	hide_sec=0,hide_min=0,hide_hour=0,hide_day=0,hide_week=0,hide_month=0,hide_year=0;
	Second=Read1302(DS1302_SECOND);
    Write1302(0x8e,0x00); //写入允许
	Write1302(0x80,Second&0x7f);
	Write1302(0x8E,0x80);          //禁止写入
	done=0;           
	while(out==0);
	while(wireless_1==1);
  }
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Upkey()//升序按键
{	   
		Up=1;
		if(Up==0||wireless_2==1)
		          {
				   mdelay(8);
				       switch(count)
				          {case 1:
                                  temp=Read1302(DS1302_SECOND);  //读取秒数
								  temp=temp+1;  //秒数加1
                                  up_flag=1;    //数据调整后更新标志
								  if((temp&0x7f)>0x59)   //超过59秒,清零
                                  temp=0;								  
								  break;
				           case 2:
                                  temp=Read1302(DS1302_MINUTE);  //读取分数
								  temp=temp+1;  //分数加1
                                  up_flag=1;
								  if(temp>0x59)          //超过59分,清零
								  temp=0;
								  break;
				           case 3:
                                  temp=Read1302(DS1302_HOUR);  //读取小时数
								  temp=temp+1;  //小时数加1
                                  up_flag=1;
								  if(temp>0x23)   //超过23小时,清零
								  temp=0;
								  break;
				           case 4:
                                  temp=Read1302(DS1302_WEEK);  //读取星期数
								  temp=temp+1;  //星期数加1
                                  up_flag=1;
								  if(temp>0x7)  
								  temp=1;
								  break;
				           case 5:
                                  temp=Read1302(DS1302_DAY);  //读取日数
								  temp=temp+1;  //日数加1
                                  up_flag=1;
								  if(temp>0x31)
								  temp=1;
								  break;
				           case 6:
                                  temp=Read1302(DS1302_MONTH);  //读取月数
								  temp=temp+1;  //月数加1
                                  up_flag=1;
								  if(temp>0x12)
								  temp=1;
								  break;
				           case 7:
                                  temp=Read1302(DS1302_YEAR);  //读取年数
								  temp=temp+1;  //年数加1
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

void Downkey()//降序按键
{	    
		Down=1;
	    if(Down==0||wireless_3==1)
		          {
				   mdelay(8);
				     switch(count)
				          {case 1:
                                  temp=Read1302(DS1302_SECOND);  //读取秒数
								  temp=temp-1;						    //秒数减1
                                  down_flag=1;       //数据调整后更新标志
								  if(temp==0x7f)     //小于0秒,返回59秒
								  temp=0x59;
								  break;
				           case 2:
                                  temp=Read1302(DS1302_MINUTE);  //读取分数
								  temp=temp-1;  //分数减1
                                  down_flag=1;
								  if(temp==-1)
								  temp=0x59;      //小于0秒,返回59秒
								  break;
				           case 3:
                                  temp=Read1302(DS1302_HOUR);  //读取小时数
								  temp=temp-1;  //小时数减1
                                  down_flag=1;
								  if(temp==-1)
								  temp=0x23;
								  break;
				           case 4:
                                  temp=Read1302(DS1302_WEEK);  //读取星期数
								  temp=temp-1;  //星期数减1
                                  down_flag=1;
								  if(temp==0)
								  temp=0x7;;
								  break;
				           case 5:
                                  temp=Read1302(DS1302_DAY);  //读取日数
								  temp=temp-1;  //日数减1
                                  down_flag=1;
								  if(temp==0)
								  temp=31;
								  break;
				           case 6:
                                  temp=Read1302(DS1302_MONTH);  //读取月数
								  temp=temp-1;  //月数减1
                                  down_flag=1;
								  if(temp==0)
								  temp=12;
								  break;
				           case 7:
                                  temp=Read1302(DS1302_YEAR);  //读取年数
								  temp=temp-1;  //年数减1
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

void Setkey()//模式选择按键
{
		Set=1;
		if(Set==0||wireless_4==1)
	    {
           mdelay(8);
           count=count+1;	 //Setkey按一次,count就加1
		   done=1;			 //进入调整模式
           while(Set==0);
           while(wireless_4==1);
		 }

}

void keydone()//按键功能执行
{        uchar Second;
		 if(flag==0)    //关闭时钟,停止计时
         { Write1302(0x8e,0x00); //写入允许
           temp=Read1302(0x80);
           Write1302(0x80,temp|0x80);
	       Write1302(0x8e,0x80); //禁止写入
           flag=1;
         }
         Setkey();				            //扫描模式切换按键
		 switch(count)
		 {case 1:do					        //count=1,调整秒
		          {
                   outkey();			   //扫描跳出按钮
				   Upkey();                //扫描加按钮
				   Downkey();              //扫描减按钮
				   if(up_flag==1||down_flag==1)  //数据更新，重新写入新的数据
				   {
				   Write1302(0x8e,0x00); //写入允许
				   Write1302(0x80,temp|0x80); //写入新的秒数
				   Write1302(0x8e,0x80); //禁止写入
				   up_flag=0;
				   down_flag=0;
				   }

				   hide_sec++;          //位闪计数
				   if(hide_sec>3)
				     hide_sec=0;
                   show_time();         //液晶显示数据
				  }while(count==2);break;  
		  case 2:do					        //count=2,调整分
		          {
				   hide_sec=0;
				   outkey();
				   Upkey();
				   Downkey();
				   if(temp>0x60)
				     temp=0;
				   if(up_flag==1||down_flag==1)
				   {
				   Write1302(0x8e,0x00); //写入允许
				   Write1302(0x82,temp); //写入新的分数
				   Write1302(0x8e,0x80); //禁止写入
				   up_flag=0;
				   down_flag=0;
				   }
				   hide_min++;
				   if(hide_min>3)
				     hide_min=0;
                   show_time();
				  }while(count==3);break;
		  case 3:do					        //count=3,调整小时
		          {
                   hide_min=0; 
				   outkey();
				   Upkey();
				   Downkey();
				   if(up_flag==1||down_flag==1)
				   {
				   Write1302(0x8e,0x00); //写入允许
				   Write1302(0x84,temp); //写入新的小时数
				   Write1302(0x8e,0x80); //禁止写入
				   up_flag=0;
				   down_flag=0;
				   }
				   hide_hour++;
				   if(hide_hour>3)
				     hide_hour=0;
                   show_time();
				  }while(count==4);break;
		  case 4:do					        //count=4,调整星期
		          {
                   hide_hour=0; 
				   outkey();
				   Upkey();
				   Downkey();
				   if(up_flag==1||down_flag==1)
				   {
				   Write1302(0x8e,0x00); //写入允许
				   Write1302(0x8a,temp); //写入新的星期数
				   Write1302(0x8e,0x80); //禁止写入
				   up_flag=0;
				   down_flag=0;
				   }
				   hide_week++;
				   if(hide_week>3)
				     hide_week=0;
                   show_time();
				  }while(count==5);break;
		  case 5:do					        //count=5,调整日
		          {
				   hide_week=0; 
				   outkey();
				   Upkey();
				   Downkey();
				   if(up_flag==1||down_flag==1)
				   {
				   Write1302(0x8e,0x00); //写入允许
				   Write1302(0x86,temp); //写入新的日数
				   Write1302(0x8e,0x80); //禁止写入
				   up_flag=0;
				   down_flag=0;
				   }
				   hide_day++;
				   if(hide_day>3)
				     hide_day=0;
                   show_time();
				  }while(count==6);break;
		  case 6:do					        //count=6,调整月
		          {
                   hide_day=0; 
				   outkey();
				   Upkey();
				   Downkey();
				   if(up_flag==1||down_flag==1)
				   {
				   Write1302(0x8e,0x00); //写入允许
				   Write1302(0x88,temp); //写入新的月数
				   Write1302(0x8e,0x80); //禁止写入
				   up_flag=0;
				   down_flag=0;
				   }
				   hide_month++;
				   if(hide_month>3)
				     hide_month=0;
                   show_time();
				  }while(count==7);break;
		  case 7:do					        //count=7,调整年
		          {
                   hide_month=0; 
				   outkey();
				   Upkey();
				   Downkey();
				   if(up_flag==1||down_flag==1)
				   {
				   Write1302(0x8e,0x00); //写入允许
				   Write1302(0x8c,temp); //写入新的年数
				   Write1302(0x8e,0x80); //禁止写入
				   up_flag=0;
				   down_flag=0;
				   }
				   hide_year++;
				   if(hide_year>3)
				     hide_year=0;
                   show_time();
				  }while(count==8);break;
		  case 8: count=0;hide_year=0;  //count8, 跳出调整模式,返回默认显示状态
	              Second=Read1302(DS1302_SECOND);
                  Write1302(0x8e,0x00); //写入允许
	              Write1302(0x80,Second&0x7f);
	              Write1302(0x8E,0x80);          //禁止写入
				  done=0;
		  break; //count=7,开启中断,标志位置0并退出
		  default:break;

		 }

}


void show_time()   //液晶显示程序
{
  DS1302_GetTime(&CurrentTime);  //获取时钟芯片的时间数据
  TimeToStr(&CurrentTime);       //时间数据转换液晶字符
  DateToStr(&CurrentTime);       //日期数据转换液晶字符
  ReadTemp();                    //开启温度采集程序
  temp_to_str();                 //温度数据转换成液晶字符
  GotoXY(12,1);                  //液晶字符显示位置
  Print(TempBuffer);             //显示温度
  GotoXY(0,1);
  Print(CurrentTime.TimeString); //显示时间
  GotoXY(0,0);
  Print(CurrentTime.DateString); //显示日期
  GotoXY(15,0);
  Print(week_value);             //显示星期
  GotoXY(11,0);
  Print("zhou");	//在液晶上显示 字母 week
  Delay1ms(400);                 //扫描延时
}



main()
{
    flag=1;           //时钟停止标志
	LCD_Initial();    //液晶初始化
	Init_DS18B20( ) ;      //DS18B20初始化
	Initial_DS1302(); //时钟芯片初始化
	up_flag=0;
	down_flag=0;
	done=0;           //进入默认液晶显示
	wireless_1=0;
	wireless_2=0;
	wireless_3=0;
	wireless_4=0;
	while(1)
	{   
        while(done==1)
          keydone();    //进入调整模式
		while(done==0)
 	    {  
            show_time();                //液晶显示数据
            flag=0;                  
  		    Setkey();				 //扫描各功能键
 		}
	}
}




