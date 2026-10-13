#include <REGX52.H>
#include "DS1302.h"
#include "Timer1_Init.h"
#include "delay.h"
#include "Music.h"
#include "TIMER0_INIT.h"
#include "Matrixkey.h"
#include "Nixie.h"
#include "AT24C02.h"

unsigned char W_Hour,W_Min,W_Sec;


//√Î±Ì±‰¡ø
unsigned char min,sec,minisec;
unsigned char Run_State;

unsigned char Mode,ke,TimeSelect1,TimeFlashFlag,State,TimeSelect2,Sure;
//√Î±Ìº∆À„
void Sec_Loop(void)
{
	if(Run_State)
	{
		
		minisec++;
		if(minisec>99)
		{
			minisec=0;
			sec++;
			if(sec>=60)
			{
				min++;
				sec=0;
				if(min>=60)
				{
					min=0;
				}
			}
		}
	}
	
}

void TimeShow1(void)
{		
		Nixie_SetBuf(1,DS1302_TIME[3]/10%10);
		Nixie_SetBuf(2,DS1302_TIME[3]%10);
		Nixie_SetBuf(3,11);
		Nixie_SetBuf(4,DS1302_TIME[4]/10%10);
		Nixie_SetBuf(5,DS1302_TIME[4]%10);
		Nixie_SetBuf(6,11);
		Nixie_SetBuf(7,DS1302_TIME[5]/10%10);
		Nixie_SetBuf(8,DS1302_TIME[5]%10);
}
void TimeShow2(void)
{	
		Nixie_SetBuf(1,DS1302_TIME[0]/10%10);
		Nixie_SetBuf(2,DS1302_TIME[0]%10);
		Nixie_SetBuf(3,11);
		Nixie_SetBuf(4,DS1302_TIME[1]/10%10);
		Nixie_SetBuf(5,DS1302_TIME[1]%10);
		Nixie_SetBuf(6,11);
		Nixie_SetBuf(7,DS1302_TIME[2]/10%10);
		Nixie_SetBuf(8,DS1302_TIME[2]%10);
}

void TimeShow3(void)
{
		Nixie_SetBuf(1,min/10);
		Nixie_SetBuf(2,min%10);
		Nixie_SetBuf(3,11);
		Nixie_SetBuf(4,sec/10);
		Nixie_SetBuf(5,sec%10);
		Nixie_SetBuf(6,11);
		Nixie_SetBuf(7,minisec/10);
		Nixie_SetBuf(8,minisec%10);
}

void TimeShow4(void)
{
		Nixie_SetBuf(1,W_Hour/10);
		Nixie_SetBuf(2,W_Hour%10);
		Nixie_SetBuf(3,11);
		Nixie_SetBuf(4,W_Min/10);
		Nixie_SetBuf(5,W_Min%10);
		Nixie_SetBuf(6,11);
		Nixie_SetBuf(7,W_Sec/10);
		Nixie_SetBuf(8,W_Sec%10);
}
void TimeSet1(void)
{
	
	if(ke==3)
	{
		TimeSelect1++;
		if(TimeSelect1>2)TimeShow1();
		else{TimeShow2();}
		if(TimeSelect1>5)
		{
			TimeShow2();
			TimeSelect1=0;			
		}
				
			
	}
	if(ke==5)
	{
		DS1302_TIME[TimeSelect1]++;
		if(DS1302_TIME[0]>99){DS1302_TIME[0]=0;}//ƒÍ≈–∂œ
		if(DS1302_TIME[1]>12){DS1302_TIME[1]=1;}//‘¬≈–∂œ
		if(DS1302_TIME[1]==1||DS1302_TIME[1]==3||DS1302_TIME[1]==5 //»’≈–∂œ
			||DS1302_TIME[1]==7||DS1302_TIME[1]==8||DS1302_TIME[1]==10
			||DS1302_TIME[1]==12)
			{
				if(DS1302_TIME[2]>31){DS1302_TIME[2]=1;}
				
			}else if(DS1302_TIME[2]==2)
							{
								if(DS1302_TIME[0]%4==0)
								{
									if(DS1302_TIME[2]>29){DS1302_TIME[2]=1;}
								}
								else if(DS1302_TIME[2]>28){DS1302_TIME[2]=1;}
							}
			
		else if(DS1302_TIME[1]==4||DS1302_TIME[1]==6
						||DS1302_TIME[1]==9||DS1302_TIME[1]==11)
			{
				if(DS1302_TIME[2]>30){DS1302_TIME[2]=1;}
			}			
		if(DS1302_TIME[3]>23){DS1302_TIME[3]=0;}//–° ±≈–∂œ
		if(DS1302_TIME[4]>59){DS1302_TIME[4]=0;}//∑÷÷”≈–∂œ
		if(DS1302_TIME[5]>59){DS1302_TIME[5]=0;}//√Î≈–∂œ
		
	}
	if(ke==6)
	{
		DS1302_TIME[TimeSelect1]--;
		if(DS1302_TIME[0]<0){DS1302_TIME[0]=99;}//ƒÍ≈–∂œ
		if(DS1302_TIME[1]<1){DS1302_TIME[1]=12;}//‘¬≈–∂œ
		if(DS1302_TIME[1]==1||DS1302_TIME[1]==3||DS1302_TIME[1]==5//»’≈–∂œ
			||DS1302_TIME[1]==7||DS1302_TIME[1]==8||DS1302_TIME[1]==10//¥Û‘¬≈–∂œ
			||DS1302_TIME[1]==12)
			{
				if(DS1302_TIME[2]<1){DS1302_TIME[2]=31;}
				
			}else if(DS1302_TIME[1]==2)
							{
								if(DS1302_TIME[0]%4==0)
								{
									if(DS1302_TIME[2]<1){DS1302_TIME[2]=29;}
									if(DS1302_TIME[2]>29){DS1302_TIME[2]=1;}
								}
								else if(DS1302_TIME[2]<1){DS1302_TIME[2]=28;}
								else if(DS1302_TIME[2]>28){DS1302_TIME[2]=1;}
							}
			
		else if(DS1302_TIME[1]==4||DS1302_TIME[1]==6 //–°‘¬≈–∂œ
						||DS1302_TIME[1]==9||DS1302_TIME[1]==11)
			{
				if(DS1302_TIME[2]<1){DS1302_TIME[2]=30;}
				if(DS1302_TIME[2]>30){DS1302_TIME[2]=1;}
			}			
		if(DS1302_TIME[3]<0){DS1302_TIME[3]=23;}//–° ±≈–∂œ
		if(DS1302_TIME[4]<0){DS1302_TIME[4]=59;}//∑÷÷”≈–∂œ
		if(DS1302_TIME[5]<0){DS1302_TIME[5]=59;}//√Î≈–∂œ
		
	}
	if(TimeSelect1==0)
	{
		if(TimeFlashFlag==1){Nixie_SetBuf(1,10);Nixie_SetBuf(2,10);}
		else{Nixie_SetBuf(1,DS1302_TIME[0]/10%10);Nixie_SetBuf(2,DS1302_TIME[0]%10);}
	}
	if(TimeSelect1==1)
	{
		if(TimeFlashFlag==1){Nixie_SetBuf(4,10);Nixie_SetBuf(5,10);}
		else{Nixie_SetBuf(4,DS1302_TIME[1]/10%10);Nixie_SetBuf(5,DS1302_TIME[1]%10);}
	}
	if(TimeSelect1==2)
	{
		if(TimeFlashFlag==1){Nixie_SetBuf(7,10);Nixie_SetBuf(8,10);}
		else{Nixie_SetBuf(7,DS1302_TIME[2]/10%10);Nixie_SetBuf(8,DS1302_TIME[2]%10);}
	}
	if(TimeSelect1==3)
	{
		if(TimeFlashFlag==1){Nixie_SetBuf(1,10);Nixie_SetBuf(2,10);}
		else{Nixie_SetBuf(1,DS1302_TIME[3]/10%10);Nixie_SetBuf(2,DS1302_TIME[3]%10);}
	}
	if(TimeSelect1==4)
	{
		if(TimeFlashFlag==1){Nixie_SetBuf(4,10);Nixie_SetBuf(5,10);}
		else{Nixie_SetBuf(4,DS1302_TIME[4]/10%10);Nixie_SetBuf(5,DS1302_TIME[4]%10);}
	}
	if(TimeSelect1==5)
	{
		if(TimeFlashFlag==1){Nixie_SetBuf(7,10);Nixie_SetBuf(8,10);}
		else{Nixie_SetBuf(7,DS1302_TIME[5]/10%10);Nixie_SetBuf(8,DS1302_TIME[5]%10);}
	}
}

void TimeSet2(void)
{
	
	if(ke==3)
	{
		TimeSelect2++;
		TimeShow4();
		TimeSelect2%=3;		
	}
	if(ke==5)
	{
		if(TimeSelect2==0)W_Hour++;
		if(TimeSelect2==1)W_Min++;
		if(TimeSelect2==2)W_Sec++;
		if(W_Hour>23){W_Hour=0;}
		if(W_Min>59){W_Min=0;}
		if(W_Sec>59){W_Sec=0;}		
	}
	if(ke==6)
	{
		if(TimeSelect2==0)W_Hour--;
		if(TimeSelect2==1)W_Min--;
		if(TimeSelect2==2)W_Sec--;
		if(W_Hour<0){W_Hour=23;}
		if(W_Min<0){W_Min=59;}
		if(W_Sec<0){W_Sec=59;}		
	}
	if(TimeSelect2==0)
	{
		if(TimeFlashFlag==1){Nixie_SetBuf(1,10);Nixie_SetBuf(2,10);}
		else{Nixie_SetBuf(1,W_Hour/10%10);Nixie_SetBuf(2,W_Hour%10);}
	}
	if(TimeSelect2==1)
	{
		if(TimeFlashFlag==1){Nixie_SetBuf(4,10);Nixie_SetBuf(5,10);}
		else{Nixie_SetBuf(4,W_Min/10%10);Nixie_SetBuf(5,W_Min%10);}
	}
	if(TimeSelect2==2)
	{
		if(TimeFlashFlag==1){Nixie_SetBuf(7,10);Nixie_SetBuf(8,10);}
		else{Nixie_SetBuf(7,W_Sec/10%10);Nixie_SetBuf(8,W_Sec%10);}
	}
}

void main()
{
	Timer1_Init();
	Timer0_Init();
	DS1302_INIT();
	DS1302_SETTIME();
	W_Hour=AT24C02_ReceiveByte(0);
	W_Min=AT24C02_ReceiveByte(1);
	W_Sec=AT24C02_ReceiveByte(2);
	if(W_Hour>23||W_Hour<0||W_Min>59||W_Min<0||W_Sec>59||W_Sec<0)
	{
		W_Hour=W_Min=W_Sec=0;
	}
	
	while(1)
	{
		ke=keynumber();
		if(ke==1)
		{
			State++;
			State%=2;
			if(State==1 && Mode==2)TimeShow2();			
			TimeSelect1=0;
			TimeSelect2=0;
		}
		if(ke==2)
		{
			Mode++;
			Mode%=4;
		}
		if(ke==4)
		{
			Run_State++;
			Run_State%=2;
		}		
		if(State==1 && Mode!=3)
		{
			TimeSet1();
			if(ke==8)
			{
				DS1302_SETTIME();
				State=0;
			}
		}
		if(State==0)
		{
			DS1302_READTIME();
			switch(Mode)
			{
				case 0:TimeShow1();break;
				case 1:TimeShow2();break;
				case 2:TimeShow3();break;
				case 3:TimeShow4();break;
			
			}	
		}
		if(State==1 && Mode==3)
		{
			TimeSet2();
		}
		if(ke==8 && Mode==2)
		{
			State=0;
			min=sec=minisec=0;
		}
		if(W_Hour==DS1302_TIME[3] && W_Min==DS1302_TIME[4])
		{
			TR1=1;
			Music_Start();
		}
		else
		{
			TR1=0;
		}
		if(ke==7)
		{
			AT24C02_WriteByte(0,W_Hour);
			delay(5);
			AT24C02_WriteByte(1,W_Min);
			delay(5);
			AT24C02_WriteByte(2,W_Sec);
		}
	
	}
	
	
}
void Timero_Routine() interrupt 1  
{
	static unsigned int counter,counter1,counter2=0;

	TL0 = 0x18;				
	TH0 = 0xFC;
	counter++;
	counter1++;
	counter2++;
	if(counter>=500)
	{
		counter=0;
		TimeFlashFlag=!TimeFlashFlag;
	}
	if(counter1>10)
	{
		Sec_Loop();
		counter1=0;
	}
	if(counter>=2)
	{
		Nixie_Loop();
		counter2=0;	
	}
}

void Timer1_Routine() interrupt 3
{
	if(FreqTable[FreqSelect_Get()])
	{
		TL1 = FreqTable[FreqSelect_Get()]%256;				
		TH1 = FreqTable[FreqSelect_Get()]/256;
		Buzzer=!Buzzer;		
	}
}