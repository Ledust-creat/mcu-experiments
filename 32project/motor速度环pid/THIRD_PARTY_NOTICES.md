# 第三方代码与参考来源

| 范围 | 来源与说明 | 许可处理 |
| --- | --- | --- |
| `Libraries/` | STMicroelectronics STM32F10x 标准外设库 V3.5.0，文件头标注 2011 年版权 | 保留原文件的版权、使用条款与免责声明 |
| `Start/` | ARM CMSIS Cortex-M3 核心文件，以及 ST 启动和系统时钟文件 | 按各文件原有条款使用；不以仓库其他内容的许可覆盖 |
| `User/` | STM32F10x 中断与外设配置模板，按本工程配置使用 | 保留文件中的原有来源和版权信息 |
| `Hardware/`、`System/`、`main.c` | 学习工程的电机控制、调参集成、基础驱动和延时等代码；文件内已有注释保持原样 | 不将整个目录统一标为独立原创或重新授予第三方代码许可；参考课程及具体模块出处可由作者继续补充 |
| 电调报文说明 | [RoboMaster C620 官方说明书](https://cdn-hz.robomaster.com/robomasters/public/document/RoboMaster%20C620%20Brushless%20DC%20Motor%20Speed%20Controller%20V1.01.pdf) | 仅提供协议参考链接，未打包说明书或上位机软件 |

工程中的速度反馈滤波、PID 调参与 CAN 收发的学习过程见 [README](README.md)。公开整理不改变上游代码的版权或许可；整仓许可说明见[仓库首页](../../README.md)。
