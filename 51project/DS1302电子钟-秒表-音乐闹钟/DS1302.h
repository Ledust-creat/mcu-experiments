#ifndef __DS1302_H__
#define __DS1302_H__

void DS1302_INIT(void);
void DS1302_WRITEBYTE(unsigned char commend,Data);
unsigned char DS1302_READBYTE(unsigned char commend);
void DS1302_SETTIME();
char DS1302_TIME[];
void DS1302_READTIME();
#endif