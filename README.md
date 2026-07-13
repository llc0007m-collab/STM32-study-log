# STM32 Study Log

这是我的 STM32 学习仓库，用于保存实验工程、学习记录和问题复盘。

## 开发环境

- MCU：STM32F103C8T6
- IDE：Keil MDK 5.43
- 编译器：ArmClang 6.24
- 固件库：STM32F10x Standard Peripheral Library 3.5.0
- 调试器：ST-Link

## 仓库结构

```text
.
├── 00_Study_Log/      # 每个实验对应的学习记录
├── 01_LED_pc13/       # PC13 LED 点亮实验工程
├── .github/           # 仓库协作和 AI 审查规则
└── README.md
```

## 学习进度

| 序号 | 实验 | 状态 | 学习记录 | 工程源码 |
|---|---|---|---|---|
| 01 | PC13 LED 点亮 | 已完成 | [查看记录](00_Study_Log/2026-7-13_01_LED_pc13/01_LED_pc13.md) | [`main.c`](01_LED_pc13/user/main.c) |

## 打开工程

1. 使用 Keil 打开 `01_LED_pc13/LED_Blink.uvprojx`；
2. 检查目标器件是否为 `STM32F103C8`；
3. 编译工程并确认无错误；
4. 连接 ST-Link，下载程序并观察实验现象。

## 版本控制约定

仓库保留源码、工程配置和学习记录。`.o`、`.d`、`.axf`、`.map`、构建日志以及个人界面状态等生成文件由 `.gitignore` 排除。

## 学习记录原则

- 学习日志应链接到真实源码，避免复制后与工程代码不一致；
- 记录预期现象、实际现象、遇到的问题和解决过程；
- 涉及第三方固件库的文件保留原始版权说明；
- AI 审查规则见 [`.github/AI_REVIEW.md`](.github/AI_REVIEW.md)。

## 参考资料

- [江协科技 STM32 入门教程](https://space.bilibili.com/383400717)
- STM32F10x 标准外设库及 ST 官方参考资料
