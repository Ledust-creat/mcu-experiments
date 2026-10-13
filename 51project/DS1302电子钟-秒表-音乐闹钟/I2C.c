#include <REGX52.H>
sbit SCl=P2^1;
sbit SDA=P2^0;
/**
   *@brief I2C开始
   *@param 无
   *@retval 无
   */
void I2C_Start(void)
{
	SDA=1;
	SCl=1;
	SDA=0;
	SCl=0;
}
/**
   *@brief I2C结束
   *@param 无
   *@retval 无
   */
void I2C_Stop(void)
{
	SDA=0;
	SCl=0;
	SCl=1;
	SDA=1;
}
/**
   *@brief I2C发送一个字节
   *@param Byte 发送的字节
   *@retval 无
   */
void I2C_SendByte(unsigned char Byte)
{
	unsigned char i;
	
	
	for(i=0;i<8;i++)
	{
			SCl=0;
			SDA=Byte&(0x80>>i);
			SCl=1;
	}
	SCl=0;
}
/**
   *@brief I2C读取一个字节
   *@param 无
   *@retval 从从段读取的字节
   */
unsigned char I2C_ReadByte(void)
{
	unsigned char Byte,i;
	SCl=0;
	SDA=1;
	//从端输入SDA
	for(i=0;i<8;i++)
	{
		SCl=1;
		if(SDA)Byte|=(0x80>>i);
		SCl=0;
	}
	
	return Byte;
}
/**
   *@brief I2C发出应答
   *@param Ack，0为应答，1为非应答
   *@retval 无
   */
void I2C_SendAck(bit Ack)
{
	SCl=0;
	SDA=Ack;
	SCl=1;
	SCl=0;
}
/**
   *@brief I2C接收应答
   *@param 无
   *@retval Ack，0为应答，1为非应答
   */
unsigned char I2C_ReceiveAck(void)
{
	unsigned char Ack;
	SCl=0;
	SDA=1;
	//从端接收应答SAD
	SCl=1;
	Ack=SDA;
	SCl=0;
	return Ack;
}








