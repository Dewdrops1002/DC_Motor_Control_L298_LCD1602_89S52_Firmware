#include	"REGX52.H"
#include	"stdio.H"
#include <LCD_4bit_Interface.H>

sbit		PWM		=P2^6;
sbit		DIR		=P2^7;
sbit		KMode	=P1^0;//On/Off
sbit		KUp		=P1^1;//Up
sbit		KDown	=P1^2;//Down
sbit		KDir	=P1^3;//Direct

unsigned int	Counter,Speed,Duty;
unsigned char Direct;
unsigned char		pMode,pDown,pDir=0,pUp;

//unsigned char	Mode,Up,Down,Direct;		

#define		Fwd		0x41
#define		Rev		0x42
#define		Mode	0x01
#define		Up		0x02
#define		Down	0x04
#define		Dir		0x08	


void Delay(unsigned int t)
{
	unsigned int i;
	for(i=0;i<=t;i++);
}

//-------------
/*
Tao xung PWM co f=500Hz =>T=2ms
20 cap toc do, => Thoi gian tran Timer=2ms/20=100uS
Dung Timer0 che do 2 (8bit Auto Reload)
*/
void Configure(void)
{
	TMOD=0x02;	//Timer0 che do 2 (Tu nap lai)
	IE=0x82;	//Cho phep ngat timer0
	TH0=TL0=155;
	TR0=0;		//0 Cho phep Timer 0 chay
}


//-------------
void	Run(unsigned char Direct,unsigned int Duty1)//20 cap toc do
{
	TR0=1;
	if(Direct==Fwd)
	{
		DIR=0;
		Duty=Duty1;
	}
	
	if(Direct==Rev)
	{
		DIR=1;
		Duty=20-Duty1;
	}
}


//-------------
void Stop(void)
{
	TR0=0;
	PWM=0;
	DIR=0;
}

//-------------
void main(void)
{
	Delay(10);
	Configure();
	LcdInit();
	
	LcdClr();
	LcdGoto(0,0);
	LcdWrStr("DC Motor Control");
	
	Stop();
	Speed=5;//default
	Direct=Fwd;
	
	while(KMode); //Cho bam phim On/Off
	while(!KMode);
	LcdClr();
	while(1)
	{
		if(!KMode) 
		{
			while(!KMode); pMode=!pMode;
		}
		
		//----
		if(pMode)
		{
			Run(Direct,Speed);
			//----
			if(!KUp)
			{
				while(!KUp); if(Speed<20) Speed++;
			}
			
			//----
			if(!KDown)
			{
				while(!KDown); if(Speed >0) Speed--;
			}
			
			//----
			if(!KDir)
			{
				while(!KDir); pDir=!pDir;
			}
			
			//----
			if(pDir)	Direct=Fwd;
			else		Direct=Rev;
			
			LcdGoto(1,6); sprintf(&M[0],"Duty  %d%c  ",Speed*5,0x25);  LcdWrStr(&M[0]);	
			
			LcdGoto(0,13); 	if(Direct==Fwd){LcdWrStr("Fwd");};
			if(Direct==Rev){LcdWrStr("Rev");};
			
			LcdGoto(0,0);LcdWrStr("Run...");
		}
		//----
		else
		{
			Stop();
			LcdClr();
			LcdGoto(0,0);LcdWrStr("Stop !   ");
		}
	}
}


//-------------
void Timer0_Int(void) interrupt 1
{
	Counter++;
	if(Counter>=20)
	{
		Counter=0;
	}
	
	if(Counter<Duty) 	PWM=1;
	else				PWM=0;
}