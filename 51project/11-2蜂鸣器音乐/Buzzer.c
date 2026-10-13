#include <REGX52.H>
#include <INTRINS.H>
/**
   *@brief 蜂鸣器I声明
   *@param 无
   *@retval 无
   */
sbit Buzzer=P2^5;
/**
   *@brief 蜂鸣器私有延迟
   *@param 无
   *@retval 无
   */
void Buzzer500us(void)	//@12.000MHz
{
	unsigned char data i;

	_nop_();
	i = 247;
	while (--i);
}


/**
   *@brief 1000hz下蜂鸣器鸣响
   *@param 鸣响时间单位ms
   *@retval 无
   */
void Buzzer_Time(unsigned int ms)
{
	int i;
	for(i=0;i<ms*2;i++)
	{
		Buzzer=!Buzzer;
		Buzzer500us();
		
	}
}