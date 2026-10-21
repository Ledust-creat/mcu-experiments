#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "LightSensor.h"
#include "Oled.h"
#include "MYCAN.h"
#include "Usart.h"
#include "MOTOR.h"
#include "pid.h"
#include "AND_DT.h"
#include <stddef.h>
#include <string.h>
#include <stdint.h>
#define FILTER_ALPHA 0.2f
extern uint8_t ano_rx_flag;
int main(void)
{
	OLED_Init();
	MyCanInit();
	Motor_Init();
	MyUSART_Init(115200);
	int16_t real_speed[4] = {0,0,0,0};   // 实际转速
	int16_t filter_speed[4] = {0,0,0,0};   //  滤波后实际转速
    int16_t target_speed[4] = {1000, 0, 0, 0}; // 目标转速
	PID_t motor_speed_pid;
	int16_t out;
	PID_Init(&motor_speed_pid,4,0.1,0.1,2000,2000);//3,0.02,0.1
	OLED_ShowString(1, 1, "Kp:");      // 第一行：Kp参数
    OLED_ShowString(1, 8, "Ki:");      // 第一行：Ki参数
    OLED_ShowString(2, 1, "Kd:");      // 第二行：Kd参数
	while(1)
	{
		if (Ano_GetData(5) != 1.0f)
		{
			// 只有运行标志为1才驱动；停机或无效标志持续发送零电流。
			Motor_cmd_chassis(0, 0, 0, 0);
			PID_Reset(&motor_speed_pid);
			Delay_ms(2);
		}
		else
		{
			for (uint8_t i = 0; i < 4; i++) 
			{
				const motor_measure_t* motor = get_chassis_motor_measure_point(i);
				if (motor != NULL) 
				{ 
//				real_speed[i] = motor->speed_rpm;
					filter_speed[i] = FILTER_ALPHA * motor->speed_rpm + (1 - FILTER_ALPHA) * filter_speed[i];//滤波后数据
					real_speed[i] = filter_speed[i];
				}
			}
			out = PID_Calc(&motor_speed_pid, target_speed[0], real_speed[0]);				
			Motor_cmd_chassis(out, 0, 0,0);
			OLED_ShowNum(4,1,out,6);
			Ano_SendM3508Data(real_speed, target_speed);
			if(ano_rx_flag)
			{
				ano_rx_flag=0;
				PID_SetParam(&motor_speed_pid, Ano_GetData(1)/100, Ano_GetData(2)/100, Ano_GetData(3)/100,2000, 2000);
				target_speed[0]=Ano_GetData(4);
			}
			
					
			Delay_ms(2);		
			OLED_ShowNum(1, 4, (int)(motor_speed_pid.Kp * 100), 3);  
			OLED_ShowNum(1, 11, (int)(motor_speed_pid.Ki * 100), 3); 
			OLED_ShowNum(2, 4, (int)(motor_speed_pid.Kd * 100), 3); 

		}
	}
}
