#ifndef __MOTOR_H
#define __MOTOR_H
#include "MYCAN.h"
#include <stddef.h>
#include <string.h>
#include <stdint.h>

/*  CAN电机ID定义 */ 
typedef enum
{
    CAN_CHASSIS_ALL_ID = 0x200,
    CAN_3508_M1_ID = 0x201,
    CAN_3508_M2_ID = 0x202,
    CAN_3508_M3_ID = 0x203,
    CAN_3508_M4_ID = 0x204,


}motor_measure_id;

/* 电机测量数据结构 */ 
typedef struct
{
    uint16_t ecd;            
    int16_t  speed_rpm;
    int16_t  given_current;
    uint8_t  temperate;
    int16_t  last_ecd;
} motor_measure_t;
/**
 * @brief  初始化电机模块
 * @param  无
 */	
void Motor_Init(void);
/**
 * @brief  获取指定底盘电机的测量数据指针（只读）
 * @param  i: 电机索引（0~3，分别对应电机1~4）
 * @retval 电机测量结构体的常量指针（防止外部修改，仅允许读取）
 * @note   通过i & 0x03确保索引不会超出0~3范围（防止越界）
 */
const motor_measure_t *get_chassis_motor_measure_point(uint8_t i);
/**
 * @brief  向4个3508电机发送控制电流指令
 * @param  motor1: 电机1（对应ID 0x201）的控制电流，范围[-16384, 16384]（对应-20A-20A）
 * @param  motor2: 电机2（对应ID 0x202）的控制电流，范围[-16384, 16384]
 * @param  motor3: 电机3（对应ID 0x203）的控制电流，范围[-16384, 16384]
 * @param  motor4: 电机4（对应ID 0x204）的控制电流，范围[-16384, 16384]
 */
void Motor_cmd_chassis(int16_t motor1, int16_t motor2, int16_t motor3, int16_t motor4);
/**
 * @brief  发送复位指令（ID 0x700），用于设置3508电机进入快速配置ID模式
 * @note   发送空数据帧（8字节全0），触发电机进入ID配置模式
 */
void Motor_cmd_chassis_reset_ID(void);






#endif
