/************************
	 引脚定义
	 RS  RW  EN    1.0 1.1  1.2
	 CS  CLK  DIO   2.0  2.1  2.2
************************/
#include <intrins.h>
#include <reg51.h>

#define ulong unsigned long
#define uint unsigned int
#define uchar unsigned char

unsigned char word1[16]={"CH = "};

sbit ADCS =P2^0;  //ADC0832 chip seclect//芯片的CS引脚
sbit ADDI =P2^2;  //ADC0832 data in		 //芯片的DIO引脚
sbit ADDO =P2^2;  //ADC0832 data out	  //
sbit ADCLK =P2^1;  //ADC0832 clock signal	//芯片的CLK引脚

	  sbit rs=P1^0;
sbit rw=P1^1;
sbit en=P1^2;  




unsigned char readad[2];


void  Adc0832(unsigned char channel);

float tt;


















/*
 1602液晶专用延时
*/ 

 void DelayUs2x(unsigned char t)
{   
 while(--t);
}

void DelayMs(unsigned char t)
{
     
 while(t--)
 {
     //大致延时1mS
     DelayUs2x(245);
	 DelayUs2x(245);
 }
}


void shuaxin(uint shuaxint)//刷新显示程序
{
while(shuaxint--);
}
//1602液晶判忙函数
 bit LCD_Check_Busy(void) 
 { 
 P0=0xFF; 
 rs=0; 
 rw=1; 
 en=0; 
 _nop_(); 
 en=1;
 return (bit)(P0 & 0x80);
 }

//写入命令函数
  void LCD_Write_Com(uchar com) 
 {  
 DelayMs(5);
 rs=0; 
 rw=0;; 
 en=1; 
 P0= com; 
 _nop_(); 
 en=0;
 }

 //写入数据函数
   void LCD_Write_Data(uchar Data) 
 { 
 DelayMs(5);
 rs=1; 
 rw=0; 
 en=1; 
 P0= Data; 
 _nop_();
 en=0;
 }

 //	LCD1602清屏函数
   void LCD_Clear(void) 
 { 
 LCD_Write_Com(0x01); 
 DelayMs(5);
 }

 //	  LCD1602写入字符函数
  void LCD_Write_Char(unsigned char x,unsigned char y,unsigned char Data) 
 {     
 if (y == 0) 
 	{     
 	LCD_Write_Com(0x80 + x);     
 	}    
 else 
 	{     
 	LCD_Write_Com(0xC0 + x);     
 	}        
 LCD_Write_Data( Data);  
 }

 //LCD1602写入字符串函数
   void LCD_Write_String(unsigned char x,unsigned char y,unsigned char *s) //写入字符串的函数
 {     
 if (y == 0) 
 	{     
	 LCD_Write_Com(0x80 + x);     //表示第一行
 	}
 else 
 	{      
 	LCD_Write_Com(0xC0 + x);      //表示第二行
 	}        
 while (*s) 
 	{     
 LCD_Write_Data( *s);     
 s ++;     
 	}
 }

 //LCD1602初始化函数
   void LCD_Init(void) 
 {
   LCD_Write_Com(0x38);    /*显示模式设置*/ 
   DelayMs(5); 
   LCD_Write_Com(0x38); 
   DelayMs(5); 
   LCD_Write_Com(0x38); 
   DelayMs(5); 
   LCD_Write_Com(0x38);  
   LCD_Write_Com(0x08);    /*显示关闭*/ 
   LCD_Write_Com(0x01);    /*显示清屏*/ 
   LCD_Write_Com(0x06);    /*显示光标移动设置*/ 
   DelayMs(5); 
   LCD_Write_Com(0x0C);    /*显示开及光标设置*/
   }










/************
读ADC0832函数
************/

//采集并返回
void Adc0832(unsigned char channel)
{
 	uchar i=0;
	uchar j;
	uint dat=0;
	uchar ndat=0;

	if(channel==0)channel=2;
	if(channel==1)channel=3;
	ADDI=1;
	_nop_();
	_nop_();
	ADCS=0;//拉低CS端
	_nop_();
	_nop_();
	ADCLK=1;//拉高CLK端
	_nop_();
	_nop_();
	ADCLK=0;//拉低CLK端,形成下降沿1
	_nop_();
	_nop_();
	ADCLK=1;//拉高CLK端
	ADDI=channel&0x1;
	_nop_();
	_nop_();
	ADCLK=0;//拉低CLK端,形成下降沿2
	_nop_();
	_nop_();
	ADCLK=1;//拉高CLK端
	ADDI=(channel>>1)&0x1;
	_nop_();
	_nop_();
	ADCLK=0;//拉低CLK端,形成下降沿3
	ADDI=1;//控制命令结束 
	_nop_();
	_nop_();
	dat=0;
	for(i=0;i<8;i++)
	{
		dat|=ADDO;//收数据
		ADCLK=1;
		_nop_();
		_nop_();
		ADCLK=0;//形成一次时钟脉冲
		_nop_();
		_nop_();
		dat<<=1;
		if(i==7)dat|=ADDO;
	}  
	for(i=0;i<8;i++)
	{
		j=0;
		j=j|ADDO;//收数据
		ADCLK=1;
		_nop_();
		_nop_();
		ADCLK=0;//形成一次时钟脉冲
		_nop_();
		_nop_();
		j=j<<7;
		ndat=ndat|j;
		if(i<7)ndat>>=1;
	}
	ADCS=1;//拉低CS端
	ADCLK=0;//拉低CLK端
	ADDO=1;//拉高数据端,回到初始状态
	readad[0]=dat;
	readad[1]=ndat;
}

void Tempprocess()
{
	tt=readad[0]/255.0*5.0;
	word1[4]=(unsigned char )(tt);
	word1[5]='.';
	word1[6]=(unsigned char )(tt*10-word1[4]*10);
	word1[7]=(unsigned char )(tt*100-word1[4]*100-word1[6]*10);
	word1[8]=(unsigned char )(tt*1000-word1[4]*1000-word1[6]*100-word1[7]*10);
	word1[9]=(unsigned char )(tt*10000-word1[4]*10000-word1[6]*1000-word1[7]*100-word1[8]*10);
	word1[4]+=48;
	word1[6]+=48;
	word1[7]+=48;
	word1[8]+=48;
	word1[9]+=48;
	word1[10]='V';
}

 void main()
 {
 
				 	LCD_Init(); 
	LCD_Clear();//清屏

 while(1)
 {
   Adc0832(0);
				Tempprocess();

LCD_Write_String(0,0,"Dian ya shu ju:");
LCD_Write_Char(4,1,word1[4]);
LCD_Write_Char(5,1,'.');
LCD_Write_Char(6,1,word1[6]);
LCD_Write_Char(7,1,word1[7]);
LCD_Write_Char(8,1,'V');



			


 }
 }

 
/*串行通讯中断，收发完成将进入该中断*/
void Serial_interrupt() interrupt 4 
{
if(RI)
{
	RI=0;//接收中断信号清零，表示将继续接收

}
} 
