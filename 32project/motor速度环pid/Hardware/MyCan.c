#include "stm32f10x.h"                  // Device header
#include "MYCAN.h"

static CAN_RxFifo0CallbackTypeDef CAN_RxFifo0MsgPendingCallback = NULL;	
/**
 * @brief  CAN1初始化函数
 * @param  无
 */
void MyCanInit(void)
{
	//使能时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1,ENABLE);
	
	//CANTX和RXGPIO初始化
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode 	=	GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin 	=	GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed 	= 	GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode 	=	GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin 	=	GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed 	= 	GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	//can1初始化
	CAN_InitTypeDef CAN_InitStructure;
	CAN_InitStructure.CAN_Prescaler = 4;
	CAN_InitStructure.CAN_BS1 = CAN_BS1_6tq;
	CAN_InitStructure.CAN_BS2 = CAN_BS2_2tq;
	CAN_InitStructure.CAN_SJW = CAN_SJW_2tq;
	CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;//CAN_Mode_LoopBack
	CAN_InitStructure.CAN_ABOM = DISABLE;
	CAN_InitStructure.CAN_AWUM = DISABLE;
	CAN_InitStructure.CAN_TTCM = DISABLE;
	CAN_InitStructure.CAN_TXFP = DISABLE;
	CAN_InitStructure.CAN_NART = DISABLE;
	CAN_InitStructure.CAN_RFLM = DISABLE;
	CAN_Init(CAN1,&CAN_InitStructure);
	
	CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);
	
	
	
	//中断配置 
	NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel =  USB_LP_CAN1_RX0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
	
	

	//配置过滤器接受所有数据
	CAN_FilterInitTypeDef CAN_Filter_InitStructure;
	CAN_Filter_InitStructure.CAN_FilterNumber = 0;
	CAN_Filter_InitStructure.CAN_FilterActivation = ENABLE;
	CAN_Filter_InitStructure.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;
	CAN_Filter_InitStructure.CAN_FilterIdHigh = 0x00000000;
	CAN_Filter_InitStructure.CAN_FilterIdLow = 0x00000000;
	CAN_Filter_InitStructure.CAN_FilterMaskIdHigh = 0x00000000;
	CAN_Filter_InitStructure.CAN_FilterMaskIdLow = 0x00000000;
	CAN_Filter_InitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
	CAN_Filter_InitStructure.CAN_FilterScale = CAN_FilterScale_32bit;	
	CAN_FilterInit(&CAN_Filter_InitStructure);
	
	
	
}
/**
 * @brief  注册CAN中断回调函数
 * @param  user_callback: 用户接收数据处理回调函数
 */
void CAN_IRQ_RegisterCallback(CAN_RxFifo0CallbackTypeDef user_callback)
{
	CAN_RxFifo0MsgPendingCallback=user_callback;
}
/*  CAN1接收中断服务函数 */
void USB_LP_CAN1_RX0_IRQHandler(void)
{
	
	
	 if (CAN_GetITStatus(CAN1, CAN_IT_FMP0) != RESET)
    {
		CanRxMsg rx_msg;
		CAN_Receive(CAN1, CAN_FIFO0, &rx_msg);
		
		CanFrame frame;// 转换为通用CanFrame
		frame.std_id = rx_msg.StdId;
        frame.len = rx_msg.DLC;
        for (uint8_t i = 0; i < rx_msg.DLC; i++) 
		{
            frame.data[i] = rx_msg.Data[i];
        }
		if (CAN_RxFifo0MsgPendingCallback != NULL)
		{
            CAN_RxFifo0MsgPendingCallback(&frame); // 调用业务层回调
        }
    }

        CAN_ClearITPendingBit(CAN1, CAN_IT_FMP0);

}

/**
 * @brief  CAN发送数据（封装硬件发送逻辑）
 * @param  frame: motor数据结构体指针
 */
uint8_t MyCan_SendFrame(const CanFrame* frame) 
{
    if (frame == NULL || frame->len > 8) return 0;
    
    CanTxMsg tx_msg;
    tx_msg.StdId = frame->std_id;
    tx_msg.IDE = CAN_Id_Standard;
    tx_msg.RTR = CAN_RTR_Data;
    tx_msg.DLC = frame->len;
    for (uint8_t i = 0; i < frame->len; i++) {
        tx_msg.Data[i] = frame->data[i];
    }
    
    uint8_t mailbox = CAN_Transmit(CAN1, &tx_msg);
    uint32_t timeout = 0xFFFF;
    while (CAN_TransmitStatus(CAN1, mailbox) != CAN_TxStatus_Ok && timeout--);
    return (timeout>0)?1:0;
}

