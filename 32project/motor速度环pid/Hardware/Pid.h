#ifndef __PID_H
#define __PID_H

#include <stdint.h>
#include <math.h>

#define PID_HIST_LEN 3        
/* PID结构体 */
typedef struct {
    /* ---- 用户参数 ---- */
    float Kp, Ki, Kd;
    float max_iout;   // 积分限幅
    float max_out;    // 总输出限幅

    /* ---- 分项输出 ---- */
    float Pout;
    float Iout;
    float Dout;
    float out;        // 总输出

    /* ---- 滑动窗口 ---- */
    float error[PID_HIST_LEN]; 
    float Dbuf[PID_HIST_LEN];    // （0最新，1上次，2上上次）

} PID_t;
/**
 * @brief  PID初始化函数
 * @param  pid: PID结构体指针
 * @param  kp: P值
 * @param  ki: I值
 * @param  kd: D值
 * @param  max_iout: 积分最大输出
 * @param  max_out: 最大输出
 */
void PID_Init(PID_t *pid, float kp, float ki, float kd, float max_iout, float max_out);
/*
*********************************************************************************************************
*   函 数 名: PID_Reset
*   功能说明: 清除控制输出与误差历史，保留增益和限幅配置
*   形    参: pid - 已初始化的有效PID实例，由主循环独占修改
*   返 回 值: 无
*********************************************************************************************************
*/
void PID_Reset(PID_t *pid);
/**
 * @brief  PID计算函数
 * @param  pid: PID结构体指针
 * @param  set: 目标值
 * @param  fdb: 反馈值
 * @retval PID计算后的输出值
 */
float PID_Calc(PID_t *pid, float set, float fdb);
/**
 * @brief  PID设置函数
 * @param  kp: P值
 * @param  ki: I值
 * @param  kd: D值
 * @param  max_iout: 积分最大输出
 * @param  max_out: 最大输出
 */
void PID_SetParam(PID_t *pid, float kp, float ki, float kd, float max_iout, float max_out);





#endif
