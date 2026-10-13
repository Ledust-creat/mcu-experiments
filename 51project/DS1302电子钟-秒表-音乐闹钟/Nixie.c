#include <REGX52.H>


	 

unsigned char Nixie_Buf[9]={0,10,10,10,10,10,10,10,10};//默认不显示
/**
   *@brief 静态数码管显示
   *@param location显示位置1-8，num显示数字0-9,10为不显示，11为显示-
   *@retval 无
   */
void Nixie_SetBuf(unsigned int location,unsigned int num)
{
	Nixie_Buf[location]=num;
}
	
void Nixie_Scan(unsigned int location,unsigned int num)
{
	P0=0x00;
	switch(location)
	{
		case 8:P2_4=0;P2_3=0;P2_2=0;break;
		case 7:P2_4=0;P2_3=0;P2_2=1;break;
		case 6:P2_4=0;P2_3=1;P2_2=0;break;
		case 5:P2_4=0;P2_3=1;P2_2=1;break;
		case 4:P2_4=1;P2_3=0;P2_2=0;break;
		case 3:P2_4=1;P2_3=0;P2_2=1;break;
		case 2:P2_4=1;P2_3=1;P2_2=0;break;
		case 1:P2_4=1;P2_3=1;P2_2=1;break;
	}
	
	switch(num)
	{
		case 0:P0=0x3F;break;
		case 1:P0=0x06;break;
		case 2:P0=0x5B;break;
		case 3:P0=0x4F;break;
		case 4:P0=0x66;break;
		case 5:P0=0x6D;break;
		case 6:P0=0x7D;break;
		case 7:P0=0x07;break;
		case 8:P0=0x7F;break;
		case 9:P0=0x6F;break;
		case 10:P0=0x00;break;
		case 11:P0=0x40;break;
	}
//	P0=0x00;
	
}

void Nixie_Loop(void)//定时器调用2ms调用一次以保证显示
{
	static unsigned char i=1;
	Nixie_Scan(i,Nixie_Buf[i]);
	i++;
	if(i>8)i=1;
}