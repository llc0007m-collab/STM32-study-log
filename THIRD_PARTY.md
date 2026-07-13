# 第三方代码说明

本仓库中的部分文件来自 STMicroelectronics 提供的 STM32F10x Standard Peripheral Library 与 CMSIS 支持文件。

## STM32F10x Standard Peripheral Library

- 版本：3.5.0
- 提供方：STMicroelectronics
- 主要目录：`01_LED_pc13/Libriry/`
- 相关配置：`01_LED_pc13/user/stm32f10x_conf.h`

## CMSIS 与启动文件

- 提供方：Arm / STMicroelectronics
- 主要目录：`01_LED_pc13/start/`

第三方文件保留其原始文件头、版权声明和许可说明。仓库作者编写或整理的内容主要包括用户代码、工程配置和学习日志。

> 当前工程目录中的 `Libriry` 为历史拼写。后续若统一重命名为 `Library`，应同步修改 Keil 工程文件中的 Include Path 和 FilePath，避免工程失效。
