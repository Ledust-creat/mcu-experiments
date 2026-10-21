#include "MOTOR.h"

static motor_measure_t  motor_chassis[4];// 静态全局变量：存储4个底盘电机的测量数据（位置、转速、电流等）

//motor data read
/**
 * @brief  从CAN接收的字节数组中解析电机测量数据，并更新到电机结构体
 * @param  ptr: 电机测量结构体指针（存储解析后的数据）
 * @param  data: CAN接收的原始数据数组
 * @note   数据格式约定：
 *         data[0]~data[1]：编码器值（16位，高8位在前）
 *         data[2]~data[3]：转速（16位，单位rpm，高8位在前）
 *         data[4]~data[5]：给定电流（16位，单位mA，高8位在前）
 *         data[6]：温度（8位，单位℃）
 */
#define get_motor_measure(ptr, data)                                    \
    {                                                                   \
        (ptr)->last_ecd = (ptr)->ecd;                                   \
        (ptr)->ecd = (uint16_t)((data)[0] << 8 | (data)[1]);            \
        (ptr)->speed_rpm = (uint16_t)((data)[2] << 8 | (data)[3]);      \
        (ptr)->given_current = (uint16_t)((data)[4] << 8 | (data)[5]);  \
        (ptr)->temperate = (data)[6];                                   \
    }

	
	
/**
 * @brief  接收中断的用户回调函数（处理底盘电机反馈数据）
 * @note   当CAN1的FIFO0接收到报文时，中断服务函数会调用此回调
 *         仅处理3508电机的反馈报文（ID范围：CAN_3508_M1_ID ~ CAN_3508_M4_ID）
 */	
static void Motor_CAN_RxCallback(const CanFrame* frame)
{

	if (frame == NULL) return;
	// 根据电机ID解析数据
    switch (frame->std_id)
    {
        case CAN_3508_M1_ID:
        case CAN_3508_M2_ID:
        case CAN_3508_M3_ID:
        case CAN_3508_M4_ID:

        {
            static uint8_t i = 0;
            // 计算电机索引
            i = frame->std_id - CAN_3508_M1_ID;
            get_motor_measure(&motor_chassis[i], frame->data);
            break;
        }
        default:
        {
            break;
        }
    }

}
/**
 * @brief  初始化电机模块
 * @param  无
 */	
void Motor_Init(void) {
    memset(motor_chassis, 0, sizeof(motor_chassis));
    CAN_IRQ_RegisterCallback(Motor_CAN_RxCallback);  // 注册CAN接收回调
}
/**
 * @brief  获取指定底盘电机的测量数据指针（只读）
 * @param  i: 电机索引（0~3，分别对应电机1~4）
 * @retval 电机测量结构体的常量指针（防止外部修改，仅允许读取）
 * @note   通过i & 0x03确保索引不会超出0~3范围（防止越界）
 */
const motor_measure_t *get_chassis_motor_measure_point(uint8_t i)
{
    return &motor_chassis[(i & 0x03)];
}

/**
 * @brief  向4个3508电机发送控制电流指令
 * @param  motor1: 电机1（对应ID 0x201）的控制电流，范围[-16384, 16384]（对应-20A-20A）
 * @param  motor2: 电机2（对应ID 0x202）的控制电流，范围[-16384, 16384]
 * @param  motor3: 电机3（对应ID 0x203）的控制电流，范围[-16384, 16384]
 * @param  motor4: 电机4（对应ID 0x204）的控制电流，范围[-16384, 16384]
 */
void Motor_cmd_chassis(int16_t motor1, int16_t motor2, int16_t motor3, int16_t motor4)
{
	CanFrame frame;
	frame.std_id = CAN_CHASSIS_ALL_ID;
    frame.len = 0x08;
    frame.data[0] = (motor1 >> 8) & 0xFF;
    frame.data[1] = motor1 & 0xFF;
    frame.data[2] = (motor2 >> 8) & 0xFF;
    frame.data[3] = motor2 & 0xFF;
    frame.data[4] = (motor3 >> 8) & 0xFF;
    frame.data[5] = motor3 & 0xFF;
    frame.data[6] = (motor4 >> 8) & 0xFF;
    frame.data[7] = motor4 & 0xFF;
    MyCan_SendFrame(&frame);  
	
	

}
/**
 * @brief  发送复位指令（ID 0x700），用于设置3508电机进入快速配置ID模式
 * @note   发送空数据帧（8字节全0），触发电机进入ID配置模式
 */
void Motor_cmd_chassis_reset_ID(void)
{
	CanFrame frame;
    frame.std_id = 0x700;
    frame.len = 0x8;
    
    frame.data[0] = 0x00;
    frame.data[1] = 0x00;
    frame.data[2] = 0x00;
    frame.data[3] = 0x00;
    frame.data[4] = 0x00;
    frame.data[5] = 0x00;
    frame.data[6] = 0x00;
    frame.data[7] = 0x00;
	
	MyCan_SendFrame(&frame);
}





















