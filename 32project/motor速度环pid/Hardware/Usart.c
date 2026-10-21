#include "Usart.h"
#include "stm32f10x.h"                  // Device header
#include <stddef.h>
#include <string.h>
#include <stdint.h>
#include "AND_DT.h"

/**
 * @brief  USART1初始化函数
 * @param  baudrate: 电机测量结构体指针（存储解析后的数据）波特率设置
 */
void MyUSART_Init(uint32_t baudrate) {
    // 1. 使能时钟（USART1挂APB2，GPIOA挂APB2）
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);

    // 2. 配置GPIO（PA9=TX，PA10=RX）
    GPIO_InitTypeDef GPIO_InitStruct;
    // TX：复用推挽输出（适合电机控制板的外部接线）
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStruct);
    // RX：上拉输入（抗干扰，适合电机强电磁环境）
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    // 3. 配置USART1参数
    USART_InitTypeDef USART_InitStruct;
    USART_InitStruct.USART_BaudRate = baudrate;
    USART_InitStruct.USART_WordLength = USART_WordLength_8b;
    USART_InitStruct.USART_StopBits = USART_StopBits_1;
    USART_InitStruct.USART_Parity = USART_Parity_No;
    USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART1, &USART_InitStruct);

    // 4. 配置接收中断（USART1_IRQn，优先级适配电机控制）
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    NVIC_InitTypeDef NVIC_InitStruct;
    NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2; // 低于电机CAN中断
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);


    USART_Cmd(USART1, ENABLE);
}
/**
 * @brief  串口发送数据（封装硬件发送逻辑）
 * @param  data: 数据组地址
 * @param  len: 数据个数
 */
void MyUSART_Send(const uint8_t* data, uint16_t len) {
    if (data == NULL || len == 0) return;
    for (uint16_t i = 0; i < len; i++) {
        USART_SendData(USART1, data[i]);
        while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET); // 等待发送完成
    }
}
/*  USART1中断服务函数 */
void USART1_IRQHandler(void) {
    if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) 
	{
		uint8_t rx_byte = USART_ReceiveData(USART1);		
		Ano_GetByte(rx_byte);  //接受数据调用匿名协议解析 
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
    }
}
