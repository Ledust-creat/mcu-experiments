# 第三方代码来源

| 范围 | 来源与说明 | 许可处理 |
|---|---|---|
| `FreeRTOS/` 内核及端口 | [FreeRTOS-Kernel V11.1.0](https://github.com/FreeRTOS/FreeRTOS-Kernel/tree/V11.1.0)；项目配置沿用官方示例模板后调整 | 上游使用 MIT 许可，各源码中的版权及完整许可文本保留；上游 [LICENSE.md](https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/V11.1.0/LICENSE.md) |
| `Libraries/` | STM32F10x 标准外设库 V3.5.0，STMicroelectronics | 保留各文件原有版权、使用条款及免责声明 |
| `Start/`、`User/` 的模板代码 | STM32F10x 启动、CMSIS 与异常模板，来源信息保留在文件头 | 按各文件原有条款使用，不能用内核 MIT 许可覆盖这些文件 |
| `main.c` 的任务集成与项目说明 | 作者的学习集成工程；参考韦东山／百问网课程与官方示例 | 仓库当前没有为全部内容统一授予新的许可，见仓库首页 |

发布这个集成例程不改变上游代码的版权或许可。编译器、设备包、调试器设置及构建产物不作为本项目源代码发布。
