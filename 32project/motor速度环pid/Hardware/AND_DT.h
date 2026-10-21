#ifndef __ANO_COMM_H
#define __ANO_COMM_H

#include "Usart.h"
#include <stddef.h>
#include <string.h>
#include <stdint.h>

  
/**
 * @brief 发送M3508电机实时数据到上位机
 * @param  real_speed: 实时反馈数组别对应4个电机
 * @param  target_speed: 目标速度数组分别对应4个电机
 */
void Ano_SendM3508Data(const int16_t real_speed[4],const int16_t target_speed[4]);
/**
 * @brief  获取上位机写入值函数
 * @param  数据索引
 * @retval 对应数据从1开始，最大值为ANO_DARA_SIZE)
 */
float Ano_GetData(uint8_t index);
/**
 * @brief  匿名协议数据校验
 * @param  data: 收到的一帧数据
 */
void Ano_GetByte(uint8_t data);
#endif
