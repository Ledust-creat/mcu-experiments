#include "pid.h"



/**
 * @brief  PID初始化函数
 * @param  pid: PID结构体指针
 * @param  kp: P值
 * @param  ki: I值
 * @param  kd: D值
 * @param  max_iout: 积分最大输出
 * @param  max_out: 最大输出
 */
void PID_Init(PID_t *pid, float kp, float ki, float kd, float max_iout, float max_out)
{
    pid->Kp = kp;  pid->Ki = ki;  pid->Kd = kd;
    pid->max_iout = max_iout;
    pid->max_out  = max_out;
    PID_Reset(pid);
}
/*
*********************************************************************************************************
*   函 数 名: PID_Reset
*   功能说明: 清除控制输出与误差历史，避免重启时沿用停机前的积分和微分状态
*   形    参: pid - 已初始化的有效PID实例，由主循环独占修改
*   返 回 值: 无；增益和限幅配置保持不变
*********************************************************************************************************
*/
void PID_Reset(PID_t *pid)
{
    pid->Pout = pid->Iout = pid->Dout = pid->out = 0.0f;
    for (uint8_t i = 0; i < PID_HIST_LEN; ++i)
    {
        pid->error[i] = 0.0f;
        pid->Dbuf[i] = 0.0f;
    }
}
/**
 * @brief  PID计算函数
 * @param  pid: PID结构体指针
 * @param  set: 目标值
 * @param  fdb: 反馈值
 * @retval PID计算后的输出值
 */
float PID_Calc(PID_t *pid, float set, float fdb)
{
	pid->error[2] = pid->error[1];
    pid->error[1] = pid->error[0];
    pid->error[0] = set - fdb;

    pid->Pout = pid->Kp * pid->error[0];

    pid->Iout += pid->Ki * pid->error[0];
    if (pid->Iout >  pid->max_iout) pid->Iout =  pid->max_iout;
    if (pid->Iout < -pid->max_iout) pid->Iout = -pid->max_iout;

    pid->Dbuf[2] = pid->Dbuf[1];
    pid->Dbuf[1] = pid->Dbuf[0];
    pid->Dbuf[0] = pid->error[0] - pid->error[1];
    pid->Dout    = pid->Kd * pid->Dbuf[0];

    pid->out = pid->Pout + pid->Iout + pid->Dout;
    if (pid->out >  pid->max_out) pid->out =  pid->max_out;
    if (pid->out < -pid->max_out) pid->out = -pid->max_out;

    return pid->out;
}
/**
 * @brief  PID设置函数
 * @param  kp: P值
 * @param  ki: I值
 * @param  kd: D值
 * @param  max_iout: 积分最大输出
 * @param  max_out: 最大输出
 */
void PID_SetParam(PID_t *pid, float kp, float ki, float kd, float max_iout, float max_out)
{
    pid->Kp = kp;  pid->Ki = ki;  pid->Kd = kd;
    pid->max_iout = max_iout;
    pid->max_out  = max_out;
}
