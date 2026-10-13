#include <REGX52.H>
sbit CE=P3^5;
sbit IO=P3^4;
sbit SCLK=P3^6;
#define DS1302_SECOND 0x80
#define DS1302_MINUTE 0x82
#define DS1302_HOUR 	0x84
#define DS1302_DATE 	0x86
//#define DS1302_DAY 		0x8A
#define DS1302_MONTH 	0x88
#define DS1302_YEAR		0x8C
#define DS1302_WP 		0x8E

char DS1302_TIME[]={24,10,2,7,55,56
};//自定义时间
void DS1302_INIT(void)//初始化
{
	SCLK=0;
	CE=0;
}
/**
   *@brief 向DS1302写入数据
   *@param command-输入地址，Data输入数据（两位数字）
   *@retval 无
   */
void DS1302_WRITEBYTE(unsigned char command,Data)
{
	unsigned char i=0;
	CE=1;

	
	for(i=0;i<8;i++)
	{
		IO=command&(0x01<<i);
		SCLK=1;
		SCLK=0;
	}
	
	for(i=0;i<8;i++)
	{
		IO=Data&(0x01<<i);
		SCLK=1;
		SCLK=0;
	}
	
	CE=0;
	
}
/**
   *@brief 读取DS1302的值
   *@param 读取的地址
   *@retval 读取的值
   */
unsigned char DS1302_READBYTE(unsigned char command)
{
	unsigned char i,Data=0;
	command|=0x01;//最低位设置1因为读地址是写的地址最低位由0化1得到
	CE=1;
	
		for(i=0;i<8;i++)
	{
		IO=command&(0x01<<i);
		SCLK=0;
		SCLK=1;
	}
	
		for(i=0;i<8;i++)
	{
		
		SCLK=1;
		SCLK=0;
		if(IO){Data|=(0x01<<i);}
	}
	
	
	
	
	CE=0;
	IO=0;
	return Data;
}

void DS1302_SETTIME()//时间设置
{
	DS1302_WRITEBYTE(DS1302_WP,0x00);
	
	DS1302_WRITEBYTE(DS1302_YEAR,DS1302_TIME[0]/10*16+DS1302_TIME[0]%10);//十进制转bcd码
	DS1302_WRITEBYTE(DS1302_MONTH,DS1302_TIME[1]/10*16+DS1302_TIME[1]%10);
//	DS1302_WRITEBYTE(DS1302_DAY,DS1302_TIME[2]/10*16+DS1302_TIME[2]%10);
	DS1302_WRITEBYTE(DS1302_DATE,DS1302_TIME[2]/10*16+DS1302_TIME[2]%10);
	DS1302_WRITEBYTE(DS1302_HOUR,DS1302_TIME[3]/10*16+DS1302_TIME[3]%10);
	DS1302_WRITEBYTE(DS1302_MINUTE,DS1302_TIME[4]/10*16+DS1302_TIME[4]%10);
	DS1302_WRITEBYTE(DS1302_SECOND,DS1302_TIME[5]/10*16+DS1302_TIME[5]%10);
	
	DS1302_WRITEBYTE(DS1302_WP,0x80);

}

void DS1302_READTIME()//读取数值
{
	unsigned char temp=0;
	temp=DS1302_READBYTE(DS1302_YEAR);//bcd码转十进制
	DS1302_TIME[0]=temp/16*10+temp%16;
	temp=DS1302_READBYTE(DS1302_MONTH);
	DS1302_TIME[1]=temp/16*10+temp%16;
//	temp=DS1302_READBYTE(DS1302_DAY);
//	DS1302_TIME[2]=temp/16*10+temp%16;
	temp=DS1302_READBYTE(DS1302_DATE);
	DS1302_TIME[2]=temp/16*10+temp%16;
	temp=DS1302_READBYTE(DS1302_HOUR);
	DS1302_TIME[3]=temp/16*10+temp%16;
	temp=DS1302_READBYTE(DS1302_MINUTE);
	DS1302_TIME[4]=temp/16*10+temp%16;
	temp=DS1302_READBYTE(DS1302_SECOND);
	DS1302_TIME[5]=temp/16*10+temp%16;

}
