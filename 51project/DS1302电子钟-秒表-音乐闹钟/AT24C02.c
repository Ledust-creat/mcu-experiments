#include "I2C.h"
#define AT24C02_Address 0xA0
void AT24C02_WriteByte(unsigned char Address,Data)
{
	I2C_Start();
	I2C_SendByte(AT24C02_Address);
	I2C_ReceiveAck();
	I2C_SendByte(Address);
	I2C_ReceiveAck();
	I2C_SendByte(Data);
	I2C_ReceiveAck();
	I2C_Stop();
}

unsigned char AT24C02_ReceiveByte(unsigned char Address)
{
	unsigned char Data;
	I2C_Start();
	I2C_SendByte(AT24C02_Address);
	I2C_ReceiveAck();
	I2C_SendByte(Address);
	I2C_ReceiveAck();
	I2C_Start();
	I2C_SendByte(AT24C02_Address|0x01);
	I2C_ReceiveAck();
	Data=I2C_ReadByte();
	I2C_SendAck(1);
	I2C_Stop();
		
	return Data;
}