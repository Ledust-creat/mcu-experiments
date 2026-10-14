# STM32F103 标准库手动移植 FreeRTOS

这是我学习 FreeRTOS 后，向 STM32F103 标准库工程手动加入内核的最小验证例程。通过 PC13 LED 任务检查任务创建、调度器启动和阻塞延时的基本路径；当前只有一个应用任务，另有内核自动创建的空闲任务。项目没有演示任务间队列、互斥锁或电机控制。

## 环境与运行行为

| 项目 | 配置 | 条件与说明 |
|---|---|---|
| MCU | STM32F103C8，Cortex-M3 | 工程按 64 KiB Flash、20 KiB SRAM 配置 |
| 时钟 | HSE 8 MHz，PLL 后 HCLK 72 MHz | 由现有 `system_stm32f10x.c` 配置；进入 main 后检查实际 HCLK |
| 工具链 | Keil MDK、ARM Compiler 5 | 本次验证使用 MDK 5.39、ARMCC 5.06 update 7 build 960；当前 RVDS 端口面向 ARMCC5 |
| 内核 | FreeRTOS V11.1.0 | 版本依据 `FreeRTOS/include/task.h` |
| 端口及内存 | `portable/RVDS/ARM_CM3`、`heap_4.c` | 只把一种 heap 实现加入编译；保留原有 17 KiB 内核堆配置 |
| LED | PC13，低电平点亮的板载 LED | 若开发板没有该 LED，应按实际电路修改 GPIO 或接入合适的外部 LED 电路 |
| 任务参数 | 优先级 2，栈深度 128 words | 本端口约 512 字节栈空间；不含任务控制块，未进行栈余量实测 |
| 运行表现 | 点亮约 500 ms，熄灭约 500 ms | Tick 为 1000 Hz；`vTaskDelay` 为相对阻塞延时，完整闪烁周期约 1 s |

## 工程与构建

| 路径 | 作用 |
|---|---|
| `main.c` | GPIO、LED 任务、启动检查及诊断钩子 |
| `Start/`、`User/`、`Libraries/` | Cortex-M3 启动文件、异常文件与 STM32 标准外设库 |
| `FreeRTOS/*.c`、`FreeRTOS/include/` | 内核源码、头文件及本项目 `FreeRTOSConfig.h` |
| `FreeRTOS/portable/RVDS/ARM_CM3/` | ARMCC5 的 Cortex-M3 端口 |
| `FreeRTOS/portable/MemMang/heap_4.c` | 动态内存管理实现 |
| `project.uvprojx` | 使用相对路径的独立 Keil 工程 |

打开 `project.uvprojx`，确认安装 STM32F1 设备包并选择 ARM Compiler 5，然后执行 Rebuild。源码中显式引用的依赖均随本目录提交，目标设备包由本机 Keil 安装；工程未锁定已失效的旧编译器版本。若本机默认选择 ARM Compiler 6，需要在 Target 页面改选 ARM Compiler 5，当前工程不宣称已适配 ARMCLANG。

下载时根据自己的硬件设置 ST-Link／J-Link／CMSIS-DAP 或其他下载方式。本仓库不上传个人 `.uvoptx` 调试器配置，不能直接假定另一台电脑使用相同的下载器。

## 移植关键点与排错

启动向量表中的 `SVC_Handler`、`PendSV_Handler`、`SysTick_Handler` 通过配置宏映射到 FreeRTOS 端口，`User/stm32f10x_it.c` 中同名空函数保持禁用。此例由 FreeRTOS 使用 SysTick，不应直接加入另一套会重新配置 SysTick 的裸机延时函数。

STM32F103 实现了 4 个 NVIC 优先级位，工程显式设置 `NVIC_PriorityGroup_4`。内核异常优先级使用 `15 << 4`，允许调用 FromISR API 的边界为 `11 << 4`；以后加入外设中断时，调用这类 API 的中断必须使用库优先级 11～15，并且只能调用对应的 FromISR 接口。该条件与任务优先级 2 是不同的概念。参考 [FreeRTOS 官方 Cortex-M 中断优先级说明](https://www.freertos.org/Documentation/02-Kernel/03-Supported-devices/04-Demos/ARM-Cortex/RTOS-Cortex-M3-M4)。

整理版本增加任务创建返回值检查、实际主频检查、内存申请失败钩子、栈溢出检查和 `configASSERT`。故障时关闭中断、将 LED 置为高电平并停在 `Error_Stop`，可以用调试器观察：

| `g_fault_code` | 含义 | 检查方向 |
|---|---|---|
| 0 | 尚未记录上述故障 | 不等同于所有模块均健康，需要结合任务和 LED 状态判断 |
| 1 | 实际 HCLK 与配置不一致 | 晶振、HSE 启动和 PLL 配置 |
| 2／4 | 任务创建失败／动态内存申请失败 | 堆大小和申请路径；启用 malloc 钩子时可能先进入故障 4 |
| 3 | 调度器返回 | 内核任务创建、调度器启动条件 |
| 5 | 检测到任务栈溢出 | 栈深度、局部变量和调用深度 |
| 6 | 断言失败 | 结合 `g_assert_file`、`g_assert_line` 和调用栈定位 |

钩子采用内核声明的正确参数签名，处理函数不调用阻塞或动态分配 API。它们用于入门调试，栈检查也不能发现所有内存破坏情形，当前工程没有实现自动恢复。

## 验证记录

| 检查 | 2026-10-14 结果 | 证据边界 |
|---|---|---|
| 完整重建 | ARMCC 5.06 update 7 build 960，0 错误、0 警告 | 本次实际运行 Keil Rebuild，已生成 AXF |
| 编译资源统计 | Code 6012、RO-data 268、RW-data 136、ZI-data 19240 字节 | 编译器报告；静态 RW+ZI 为 19376 字节，不能据此声称任务栈余量充足 |
| 时钟、任务与 LED | 作者于 2026-10-14 确认整理版烧录及上板功能验证成功 | 记录作者反馈；500 ms 是程序设定值，尚未提供实际波形或故障码截图 |
| 故障钩子 | 编译链接通过 | 本次未做堆耗尽、栈溢出等实机故障注入 |

17 KiB 内核堆包含动态创建任务的栈和控制块，不能在 RAM 预算中把这部分再重复相加。但它与其他全局变量、启动文件中的 MSP 栈等共同占用 20 KiB SRAM，后续增加串口缓存和其他模块前应重新核算。

后续补充运行照片或 LED 视频；需要验证时序时，用示波器或逻辑分析仪检查 PC13 两次相邻翻转的间隔，并记录 `g_fault_code`、实际测量值和连续观察时长。当前上板功能通过不代表已经完成波形测量或故障注入。

## 来源与贡献

学习参考韦东山／百问网 FreeRTOS 课程和 FreeRTOS 官方示例。内核、CMSIS 启动代码和 STM32 标准外设库来自各自上游；我的工作是将它们集成到标准库工程、完成配置与任务验证，并整理说明。此次公开整理补充了上述诊断检查，没有宣称内核和标准库为个人原创。

原学习文档以移植截图为主。博客草稿和待发布配图在本地独立管理，发布后的 CSDN 链接再补充到项目说明。上游版权与许可说明见 [第三方来源](THIRD_PARTY_NOTICES.md)。
