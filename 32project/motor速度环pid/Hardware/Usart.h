#ifndef __UART_HAL_H
#define __UART_HAL_H

#include "stm32f10x.h"
#include <stdint.h>



/**
 * @brief  USART1初始化函数
 * @param  baudrate: 电机测量结构体指针（存储解析后的数据）波特率设置
 */
void MyUSART_Init(uint32_t baudrate);

/**
 * @brief  串口发送数据（封装硬件发送逻辑）
 * @param  data: 数据组地址
 * @param  len: 数据个数
 */
void MyUSART_Send(const uint8_t* data, uint16_t len);

#endif
