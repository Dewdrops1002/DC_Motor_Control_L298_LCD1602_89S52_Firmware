#ifndef __LCD_4bit_Interface_H__
#define __LCD_4bit_Interface_H__
#include	<intrins.h>


sfr 	LcdPort     = 0x80;//Port0
sbit 	RS 			= 0x80;//P0.0
sbit 	RW 			= 0x81;//P0.1
sbit 	EN 			= 0x82;//P0.2

unsigned char M[16];
//-----------------
void LcdDelay(unsigned int t)
{
	unsigned int i;
	for(i=0;i<=t;i++);
}

//-----------------
bit LcdBusy()
{
	bit BF;
	LcdPort=0xf0;
	
	RW=1;
	RS=0;
	//
	EN=1;	LcdDelay(50);
	BF=0;	BF=LcdPort&0x80;
	EN=0;	LcdDelay(50);
	//
	EN=1;	LcdDelay(50);
	EN=0;	LcdDelay(50);
	//
	return(BF);
}


//-----------------

void LcdWrite(char d)
{  
	LcdPort=((LcdPort&0x0F)|(d&0xF0));
	EN=1;	LcdDelay(50);
	EN=0;	LcdDelay(50);
	//
	LcdPort=((LcdPort&0x0F)|((d<<4)&0xF0));
	EN=1;	LcdDelay(50);
	EN=0;	LcdDelay(50);
}


//-----------------
void LcdWrData(char LcdData)
{
	while(LcdBusy());
	RW=0;
	RS=1;
	LcdWrite(LcdData);
}

//-----------------
void LcdWrCmd(char LcdCmd)		
{
	while(LcdBusy());
	RW=0;
	RS=0;
	LcdWrite(LcdCmd);
}


//-----------------
void LcdWrStr(char *s)
{
	while(*s)
	{
		LcdWrData(*s);
		s++;
	}
}

//-----------------
void LcdClr()
{
	LcdWrCmd(0x01);
}

//-----------------
void LcdGoto(char y,char x)
{
	char N[2]={0x80,0xC0};
	LcdWrCmd(N[y]+x);
}
//-----------------
void LcdInit()
{
	RS=0;
	RW=0;
	EN=0;
	
	LcdWrCmd(0x01);
	LcdDelay(50);
	//
	LcdWrCmd(0x02);
	LcdWrCmd(0x01);
	//
	LcdDelay(50);
	LcdWrCmd(0x0c);//display on, cursor off, brink off
	//
	LcdDelay(50);
	LcdWrCmd(0x28);//interface 4 bits, 2 lines, 5x7 dots
}
//-----------------

#endif