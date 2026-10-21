#include "AND_DT.h"
#include <string.h>
/* 匿名V7上位机协议	*/

#define ANO_DARA_SIZE 5 
uint8_t ano_rx_flag=0;
static uint8_t send_buf[40]; 			/* 发送数据数组 */
static uint8_t read_buf[32];	        /* 发送回上位机读取数组 */
static uint8_t check_buf[12];	        /* 发送校验数组 */
static uint8_t Data_Get[32];			/* 接收数据数组 */

static float data[ANO_DARA_SIZE]={300,2,10,1000,0};
/*
*********************************************************************************************************
*   函 数 名: Ano_GetData
*   功能说明: 读取上位机参数，参数编号从1开始
*   形    参: index - 有效范围1～ANO_DARA_SIZE
*   返 回 值: 有效编号返回对应参数，越界编号返回0.0f
*********************************************************************************************************
*/
float Ano_GetData(uint8_t index)
{
    // 参数编号从1开始，允许读取最后一项运行标志，同时拒绝编号0。
    if (index == 0U || index > ANO_DARA_SIZE)
    {
        return 0.0f;
    }
    return data[index - 1U];
}
/**
 * @brief 发送M3508电机实时数据到上位机
 * @param  real_speed: 实时反馈数组别对应4个电机
 * @param  target_speed: 目标速度数组分别对应4个电机
 */
void Ano_SendM3508Data(const int16_t real_speed[4],const int16_t target_speed[4]) {
    // 空指针保护（避免传入无效指针导致崩溃）
    if (real_speed == NULL || target_speed == NULL) {
        return;
    }

    uint8_t i,cnt=0;	/* 计数 */
	uint8_t sc=0,ac=0;	/* 校验 */
	
    // 帧头配置
    send_buf[cnt++] = 0xAA;             // 帧头
    send_buf[cnt++] = 0xFF;    			// 控制板地址
    send_buf[cnt++] = 0xF1;				// 功能字
    send_buf[cnt++] = 0x10;              // 数据长度（24字节）
		
    // 填充M3508数据（12个int16，共24字节）
    // 电机1：转速、目标
	send_buf[cnt++] =  real_speed[0]& 0xFF;
    send_buf[cnt++] = (real_speed[0] >> 8) & 0xFF;
	send_buf[cnt++] = target_speed[0] & 0xFF;
    send_buf[cnt++] = (target_speed[0] >> 8) & 0xFF;
    
    // 电机2
    send_buf[cnt++] =  real_speed[1]& 0xFF;
    send_buf[cnt++] = (real_speed[1] >> 8) & 0xFF;
	send_buf[cnt++] = target_speed[1] & 0xFF;
    send_buf[cnt++] = (target_speed[1] >> 8) & 0xFF;
	
    // 电机3
    send_buf[cnt++] =  real_speed[2]& 0xFF;
    send_buf[cnt++] = (real_speed[2] >> 8) & 0xFF;
	send_buf[cnt++] = target_speed[2] & 0xFF;
    send_buf[cnt++] = (target_speed[2] >> 8) & 0xFF;

    // 电机4
    send_buf[cnt++] =  real_speed[3]& 0xFF;
    send_buf[cnt++] = (real_speed[3] >> 8) & 0xFF;
	send_buf[cnt++] = target_speed[3] & 0xFF;
    send_buf[cnt++] = (target_speed[3] >> 8) & 0xFF;
	
	for(i=0;i<cnt;i++)	/* 校验位计算 */
	{
		sc+=send_buf[i];
		ac+=sc;
	}
	send_buf[cnt++] = sc;
	send_buf[cnt++] = ac;

	
    // 发送到上位机
    MyUSART_Send(send_buf, cnt);
}

/**
 * @brief  上位机参数读取返回
 * @param  id: 数据id
 * @param  data: 数据
 */
static void Ano_SentPar(uint16_t id,int32_t data)
{
    uint8_t cnt=0,i=0,sc=0,ac=0;
    
	    // 帧头配置
    read_buf[cnt++] = 0xAA;             // 帧头
    read_buf[cnt++] = 0xFF;    			// 控制板地址
    read_buf[cnt++] = 0xE2;				// 功能字
	read_buf[cnt++] = 0x06;	
	
    read_buf[cnt++] = id;
	read_buf[cnt++] = (id >> 8) & 0xFF;
	
	read_buf[cnt++] = data;
	read_buf[cnt++] = (data >> 8) & 0xFF;
	read_buf[cnt++] = (data >> 16) & 0xFF;
	read_buf[cnt++] = (data >> 24) & 0xFF;
    
    
    for(i=0;i<cnt;i++)		/* 校验位计算 */
	{
		sc+=read_buf[i];
		ac+=sc;
	}
	
	read_buf[cnt++] = sc;
	read_buf[cnt++] = ac;
    
    // 发送到上位机
    MyUSART_Send(read_buf, cnt); 
}
/**
 * @brief  返回给上位机的校验数据，接受到上位机数据写入时通信使用，内部调用
 * @param  id: 数据id
 * @param  _sc: 收到数据的和校验
 * @param  _ac: 收到数据的附加校验
 */
static void Ano_SentCheck(uint8_t id,uint8_t _sc,uint8_t _ac)  /* 校验帧 */
{
    uint8_t cnt=0,i=0,sc=0,ac=0;
    
	    // 帧头配置
    check_buf[cnt++] = 0xAA;             // 帧头
    check_buf[cnt++] = 0xFF;    			// 控制板地址
    check_buf[cnt++] = 0x00;				// 功能字
	check_buf[cnt++] = 0x03;
	
    check_buf[cnt++] = id;
	check_buf[cnt++] = _sc;
    check_buf[cnt++] = _ac;
    

    
    for(i=0;i<cnt;i++)		/* 校验位计算 */
	{
		sc+=check_buf[i];
		ac+=sc;
	}
	
	check_buf[cnt++] = sc;
	check_buf[cnt++] = ac;
    
     // 发送到上位机
    MyUSART_Send(check_buf, cnt);  
}
/**
 * @brief  匿名协议数据解析函数
 * @param  Data_Pack: 数据包
 */
static void Ano_Anl(uint8_t *Data_Pack)          /* 数据解析 */
{
    uint8_t sc = 0,ac = 0;
    
    for(uint8_t i=0;i<Data_Pack[3] + 4;i++)
    {
        sc += Data_Pack[i];
        ac += sc;
    }
    if(sc != Data_Pack[Data_Pack[3]+4] || ac!= Data_Pack[Data_Pack[3]+5])
    {
        return;
    } 
    
    if(Data_Pack[2] == 0xE1) /* 参数读取ID 0XE1 */
    {
        uint16_t id = Data_Pack[4] + ((uint16_t)Data_Pack[5]<<8);
        switch(id)
        {            
            case 1: 
                Ano_SentPar(id,data[1-1]); 
            break;
            case 2:
                Ano_SentPar(id,data[2-1]);
            break; 
			case 3:
                Ano_SentPar(id,data[3-1]);
            break; 
			case 4:
                Ano_SentPar(id,data[4-1]);
            break; 
			case 5:
                Ano_SentPar(id,data[5-1]);
            break; 
            
            default:
                Ano_SentPar(id,0);
            break;
        }
    }
    
    if(Data_Pack[2] == 0xE2) /* 参数写入ID 0XE2，接收到的是整数*/
    {
        uint16_t id = Data_Pack[4] + ((uint16_t)Data_Pack[5]<<8);
        switch(id)
        {
            case 1:
                data[1-1]= (uint32_t)Data_Pack[6]|((uint32_t)Data_Pack[7] << 8)|((uint32_t)Data_Pack[8] << 16); 
            break;
            case 2:
                data[2-1]=(uint32_t)Data_Pack[6]|((uint32_t)Data_Pack[7] << 8)|((uint32_t)Data_Pack[8] << 16);
            break;
			case 3:
                data[3-1]=(uint32_t)Data_Pack[6]|((uint32_t)Data_Pack[7] << 8)|((uint32_t)Data_Pack[8] << 16);
            break;
			case 4:
                data[4-1]=(uint32_t)Data_Pack[6]|((uint32_t)Data_Pack[7] << 8)|((uint32_t)Data_Pack[8] << 16);
            break;
			case 5:
                data[5-1]=(uint32_t)Data_Pack[6]|((uint32_t)Data_Pack[7] << 8)|((uint32_t)Data_Pack[8] << 16);
            break;
            
                
        }
		ano_rx_flag=0x01; 
    }	
    Ano_SentCheck(Data_Pack[2],Data_Pack[Data_Pack[3]+4],Data_Pack[Data_Pack[3]+5]); /* 返回校验 */
}
/**
 * @brief  匿名协议数据校验
 * @param  data: 收到的一帧数据
 */
void Ano_GetByte(uint8_t data)
{
    static uint8_t sta=0,datalen=0,datacnt=0;      /* 帧长度，接收到长度 */ 
    
    if(sta==0)
    {
        datalen=0;
        datacnt=0;
        
        Data_Get[0]=data;
        if(data==0xAA)       /* 帧头 */
            sta=1;
    }
    else if(sta==1)          /* 地址 */
    {
        Data_Get[1]=data;
        sta=2;
    }
    else if(sta==2)          /* ID */
    {
        Data_Get[2]=data;
        sta=3;
    }
    else if(sta==3)          /* 数据长度 */
    {
        sta=4;
        Data_Get[3]=data;
        datalen=data;
    }
    else if(sta==4)         /* 数据缓存 */
    {
        Data_Get[4+datacnt++]=data;
        if(datacnt >= datalen)
        {
            sta=5;
        }
    }
    else if(sta==5)         /* 和校验 */
    {
        Data_Get[4+datacnt++]=data;
        sta=6;
    }
    else if(sta==6)         /* 附加校验 */
    {
        sta=0;
        Data_Get[4+datacnt++]=data;
        
        Ano_Anl(Data_Get);  /* 数据解析 */
    }     
}


