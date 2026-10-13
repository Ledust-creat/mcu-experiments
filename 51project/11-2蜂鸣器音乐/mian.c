#include <REGX52.H>
#include <delay.H>
#include <Timer0_Init.H>
#define SPEED 500
sbit Buzzer=P2^5;
unsigned int FreqSelect,MusicSelect;
#define P	0
#define L1	1
#define L1_	2
#define L2	3
#define L2_	4
#define L3	5
#define L4	6
#define L4_	7
#define L5	8
#define L5_	9
#define L6	10
#define L6_	11
#define L7	12
#define M1	13
#define M1_	14
#define M2	15
#define M2_	16
#define M3	17
#define M4	18
#define M4_	19
#define M5	20
#define M5_	21
#define M6	22
#define M6_	23
#define M7	24
#define H1	25
#define H1_	26
#define H2	27
#define H2_	28
#define H3	29
#define H4	30
#define H4_	31
#define H5	32
#define H5_	33
#define H6	34
#define H6_	35
#define H7	36
unsigned int FreqTable[]={
	0,
	63628,63731,63835,63928,64021,64103,64185,64260,64331,64400,64463,64528,
	64580,64633,64684,64732,64777,64820,64860,64898,64934,64968,65000,65030,
	65058,65085,65110,65134,65157,65178,65198,65217,65235,65252,65268,65283
};



unsigned int code Music[]={
//1
L7,1,
M1,1,
M2,1,
M3,1,
L3,2,
M5,1,
M3,1,
L2,2,
L5,2,
M2,4,
//2
L7,1,
M1,1,
M2,1,
M3,1,
L2,2,
M5,1,
M3,1,
M2,1,
M3,1,
M1,1,
M2,1,
L7,1,
M1,1,
L5,2,

L7,1,
M1,1,
M2,1,
M3,1,
L3,2,
M5,1,
M3,1,
L2,2,
L5,2,
M2,4, 

L7,1,
M1,1,
M2,1,
M3,1,
L2,2,
M5,1,
M3,1,
M2,1,
M3,1,
M1,1,
M2,1,
L7,1,
M1,1,
L5,2,

L7,1,
M1,1,
M2,1,
M3,1,
L3,2,
M5,1,
M3,1,
L2,2,
L5,2,
M2,4,

L7,1,
M1,1,
M2,1,
M3,1,
L2,2,
M5,1,
M3,1,
M2,1,
M3,1,
M1,1,
M2,1,
L7,1,
M1,1,
L5,2,

L7,1,
M1,1,
M2,1,
M3,1,
L3,2,
M5,1,
M3,1,
L2,2,
L5,2,
M2,4,

M2,8,
M1_,8,
//3
M2,3,
M1,1,
M2,3,
M1,1,
M2,2,
M3,2,
M5,2,
M3,1,
M2,1,

M2,3,
M1,1,
M2,3,
M1,1,
M2,1,
M3,1,
M2,1,
M1,1,
L5,4,

M2,3,
M1,1,
M2,3,
M1,1,
M2,2,
M3,2,
M5,2,
M3,1,
M2,1,

M2,3,
M3,1,
M2,2,
M1,1,
M2,1,
M2,8,

M2,3,
M1,1,
M2,3,
M1,1,
M2,2,
M3,2,
M5,2,
M3,1,
M2,1,

M2,3,
M3,1,
M2,2,
M1,1,
L6,1,
L6,4,
M3,1,
M2,1,
M1,1,
M2,1,

M1,4,
M3,1,
M2,1,
M1,1,
M2,1,
M1,3,
L5,1,
M3,1,
M2,1,
M1,1,
M2,1,

M1,8,
M1,2,
M2,2,
M3,2,
M1,2,

M6,2,
M5,1,
M6,4,
M1,1,
M7,2,
M6,1,
M7,5,

M7,2,
M6,1,
M7,4,
M3,1,
H1,1,
H2,1,
H1,1,
M7,1,
M6,2,
M5,2,

M6,2,
M5,1,
M6,2,
M5,1,
M6,1,
M5,1,
M6,2,
M5,1,
M2,3,
M5,1,
M3,1,

M3,8,
M1,2,
M2,2,
M3,2,
M1,2,

M6,2,
M5,1,
M6,3,
M6,1,
M7,5,
M1,1,
M7,2,

M7,2,
M6,1,
M7,4,
M3,1,
H1,1,
H2,1,
H1,1,
M7,1,
M6,2,
M5,2,

M6,2,
H3,1,
H3,3,
M5,2,
M6,2,
H3,1,
H3,3,
M5,1,
M6,1,

M6,12,
H1,2,
H2,2,
//4
H3,2,
H6,1,
H5,3,
H6,1,
H5,3,
H6,1,
H5,3,
H2,1,
H3,3,
H6,1,
H5,3,
H6,1,
H5,3,
H6,1,
H5,3,
H2,1,
H3,1,

H2,2,
H1,1,
M6,3,
H1,1,
H1,1,
H2,1,
H1,1,
M6,3,
H1,3,

H3,5,
H4,1,
H3,1,
H2,1,
H3,1,
H2,3,
H1,2,
H2,2,

H3,2,
H6,1,
H5,3,
H6,1,
H5,3,
H6,1,
H5,3,
H3,2,

H3,2,
H6,1,
H5,3,
H6,1,
H5,3,
H6,1,
H5,2,
H3,3,

H2,2,
H1,1,
M6,2,
H3,3,
H2,1,
H1,1,
M6,3,
H1,3,

H1,12,
M6,1,
H3,3,

H2,2,
H1,1,
M6,2,
H3,2,
H2,1,
H2,1,
H1,2,
M6,3,
H1,2,

H1,16,

	0xff
};


void main()
{
	Timer0_Init();

	while(1)
	{	
		FreqSelect++;
		if(Music[MusicSelect]!=0xff)
		{
				FreqSelect=Music[MusicSelect];
				MusicSelect++;
				delay(SPEED/4*Music[MusicSelect]);
				MusicSelect++;
				
				TR0=0;
				delay(5);
				TR0=1;
		}
		else
		{
			TR0=0;
			while(1);
		}

	}
	
}

void time0() interrupt 1  //1Ãëµ÷ÓÃÄ£°å
{
	
	if(FreqTable[FreqSelect])
	{
		TL0 = FreqTable[FreqSelect]%256;				
		TH0 = FreqTable[FreqSelect]/256;
		Buzzer=!Buzzer;
		
	}

}
