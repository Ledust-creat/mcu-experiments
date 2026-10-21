#ifndef __MYCAN_H
#define __MYCAN_H
#include <stddef.h>
#include <string.h>
#include <stdint.h>
/* motor数据专用结构体 */
typedef struct {
    uint32_t std_id;      // 标准ID
    uint8_t data[8];      // 数据
    uint8_t len;          // 数据长度
} CanFrame;
/* 定义接收回调函数类型 */
typedef void (*CAN_RxFifo0CallbackTypeDef)(const CanFrame* frame);
/**
 * @brief  CAN1初始化函数
 * @param  无
 */
void MyCanInit(void);
/**
 * @brief  注册CAN中断回调函数
 * @param  user_callback: 用户接收数据处理回调函数
 */
void CAN_IRQ_RegisterCallback(CAN_RxFifo0CallbackTypeDef user_callback);
/**
 * @brief  CAN发送数据（封装硬件发送逻辑）
 * @param  frame: motor数据结构体指针
 * @retval 1为发送成功，0发送超时
 */
uint8_t MyCan_SendFrame(const CanFrame* frame);

#endif
