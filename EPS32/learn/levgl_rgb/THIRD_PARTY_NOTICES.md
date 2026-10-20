# 来源与第三方组件说明

本项目是 ESP32-S3 + LVGL 的学习整合与应用记录。应用界面、板级适配、学习调试和设备集成与底层第三方组件分开说明；不声称 LVGL、ESP-IDF、官方 demo 或带有原始版权头的驱动均为个人原创，也不为整个学习仓库统一新增许可证。

| 内容 | 来源 / 当前声明 | 保留方式 |
| --- | --- | --- |
| LVGL 8.3.10 | https://github.com/lvgl/lvgl/tree/v8.3.10 | 随源码保留 `components/lvgl/LICENCE.txt` 及文件内版权 |
| LVGL demos / examples | 本地 LVGL 快照中的官方示例 | 当前主页不是 widgets demo；官方示例不计为个人界面设计成果 |
| ESP-IDF 5.3.1 | https://github.com/espressif/esp-idf/tree/v5.3.1 | 构建依赖外部 SDK，不上传本机工具链；保留生成文件与借用代码的声明 |
| WS2812 驱动 | `components/bsp/led_ws2812.c` 包含 Espressif 2021–2022、Apache-2.0 版权头 | 保留原始头部和适用声明，Apache-2.0 文本见 `licenses/Apache-2.0.txt` |
| LVGL 适配与其他设备模块 | `main/`、`components/bsp/` 中的学习实现和适配 | 保留文件中现有来源和声明；不把缺少版权头等同于可以随意重新署名 |
| UI 图标与视频 | `main/img/` 为图标 C 数组；`demo/` 为作者提供的实机视频 | 视频作为个人项目演示；图标原始来源不能仅由转换后的数组确定，不宣称原创图标设计 |

具体教程链接尚未在当前项目资料中完整记录，不凭猜测补填。文章应区分“看教程学习原理”“使用官方库/示例”和“自己完成应用与适配”，不把三者写成同一种来源。
