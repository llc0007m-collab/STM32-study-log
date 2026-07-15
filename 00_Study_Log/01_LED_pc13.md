# 01 — PC13 LED 点亮

- 学习日期：2026-07-13
- MCU：STM32F103C8T6
- 状态：已完成

## 学习目标

完成第一个 STM32 标准外设库工程，理解 GPIO 输出的基本配置流程，并驱动 PC13 上连接的 LED。

## 新建工程步骤

1. 建立工程目录，在 Keil 中新建工程并选择目标器件；
2. 创建 `start`、`Library`、`user` 等分组，将启动文件、固件库和用户代码加入对应分组；
3. 在工程的 Include Paths 中添加头文件目录；
4. 在预处理宏中定义 `USE_STDPERIPH_DRIVER`；
5. 在 Debug 设置中选择调试器；
6. 在 Flash Download 中启用下载后复位运行。

## 核心原理

1. 使用 GPIOC 前必须先通过 RCC 开启 GPIOC 的外设时钟；
2. PC13 被配置为推挽输出；
3. LED 不需要高速翻转，因此 GPIO 输出速度设置为 2 MHz；
4. 本工程按照“PC13 输出低电平时 LED 点亮”的方式驱动，具体有效电平仍应以开发板原理图为准。

## 工程源码 [`01_LED_pc13/user/main.c`](../../01_LED_pc13/user/main.c)

```c
GPIO_InitTypeDef gpio_init = {0};

RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

gpio_init.GPIO_Mode = GPIO_Mode_Out_PP;
gpio_init.GPIO_Pin = GPIO_Pin_13;
gpio_init.GPIO_Speed = GPIO_Speed_2MHz;
GPIO_Init(GPIOC, &gpio_init);

GPIO_ResetBits(GPIOC, GPIO_Pin_13);
```

感谢[江协科技](https://space.bilibili.com/383400717)的 STM32 入门教程。
