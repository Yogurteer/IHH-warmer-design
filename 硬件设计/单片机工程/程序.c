/************************
	 ���Ŷ���
	 RS  RW  EN    1.0 1.1  1.2
	 CS  CLK  DIO   2.0  2.1  2.2
************************/
#include <intrins.h>
#include <reg51.h>

#define ulong unsigned long
#define uint unsigned int
#define uchar unsigned char

unsigned char word1[16]={"CH = "};

sbit ADCS =P2^0;  //ADC0832 chip seclect//оƬ��CS����
sbit ADDI =P2^2;  //ADC0832 data in		 //оƬ��DIO����
sbit ADDO =P2^2;  //ADC0832 data out	  //
sbit ADCLK =P2^1;  //ADC0832 clock signal	//оƬ��CLK����

	  sbit rs=P1^0;
sbit rw=P1^1;
sbit en=P1^2;  




unsigned char readad[2];


void  Adc0832(unsigned char channel);

float tt;


















/*
 1602Һ��ר����ʱ
*/ 

 void DelayUs2x(unsigned char t)
{   
 while(--t);
}

void DelayMs(unsigned char t)
{
     
 while(t--)
 {
     //������ʱ1mS
     DelayUs2x(245);
	 DelayUs2x(245);
 }
}


void shuaxin(uint shuaxint)//ˢ����ʾ����
{
while(shuaxint--);
}
//1602Һ����æ����
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

//д�������
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

 //д�����ݺ���
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

 //	LCD1602��������
   void LCD_Clear(void) 
 { 
 LCD_Write_Com(0x01); 
 DelayMs(5);
 }

 //	  LCD1602д���ַ�����
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

 //LCD1602д���ַ�������
   void LCD_Write_String(unsigned char x,unsigned char y,unsigned char *s) //д���ַ����ĺ���
 {     
 if (y == 0) 
 	{     
	 LCD_Write_Com(0x80 + x);     //��ʾ��һ��
 	}
 else 
 	{      
 	LCD_Write_Com(0xC0 + x);      //��ʾ�ڶ���
 	}        
 while (*s) 
 	{     
 LCD_Write_Data( *s);     
 s ++;     
 	}
 }

 //LCD1602��ʼ������
   void LCD_Init(void) 
 {
   LCD_Write_Com(0x38);    /*��ʾģʽ����*/ 
   DelayMs(5); 
   LCD_Write_Com(0x38); 
   DelayMs(5); 
   LCD_Write_Com(0x38); 
   DelayMs(5); 
   LCD_Write_Com(0x38);  
   LCD_Write_Com(0x08);    /*��ʾ�ر�*/ 
   LCD_Write_Com(0x01);    /*��ʾ����*/ 
   LCD_Write_Com(0x06);    /*��ʾ����ƶ�����*/ 
   DelayMs(5); 
   LCD_Write_Com(0x0C);    /*��ʾ�����������*/
   }










/************
��ADC0832����
************/

//�ɼ�������
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
	ADCS=0;//����CS��
	_nop_();
	_nop_();
	ADCLK=1;//����CLK��
	_nop_();
	_nop_();
	ADCLK=0;//����CLK��,�γ��½���1
	_nop_();
	_nop_();
	ADCLK=1;//����CLK��
	ADDI=channel&0x1;
	_nop_();
	_nop_();
	ADCLK=0;//����CLK��,�γ��½���2
	_nop_();
	_nop_();
	ADCLK=1;//����CLK��
	ADDI=(channel>>1)&0x1;
	_nop_();
	_nop_();
	ADCLK=0;//����CLK��,�γ��½���3
	ADDI=1;//����������� 
	_nop_();
	_nop_();
	dat=0;
	for(i=0;i<8;i++)
	{
		dat|=ADDO;//������
		ADCLK=1;
		_nop_();
		_nop_();
		ADCLK=0;//�γ�һ��ʱ������
		_nop_();
		_nop_();
		dat<<=1;
		if(i==7)dat|=ADDO;
	}  
	for(i=0;i<8;i++)
	{
		j=0;
		j=j|ADDO;//������
		ADCLK=1;
		_nop_();
		_nop_();
		ADCLK=0;//�γ�һ��ʱ������
		_nop_();
		_nop_();
		j=j<<7;
		ndat=ndat|j;
		if(i<7)ndat>>=1;
	}
	ADCS=1;//����CS��
	ADCLK=0;//����CLK��
	ADDO=1;//�������ݶ�,�ص���ʼ״̬
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
	LCD_Clear();//����

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

 
/*����ͨѶ�жϣ��շ���ɽ�������ж�*/
void Serial_interrupt() interrupt 4 
{
if(RI)
{
	RI=0;//�����ж��ź����㣬��ʾ����������

}
} 
