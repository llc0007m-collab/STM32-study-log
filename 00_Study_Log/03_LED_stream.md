# 03_LED_stream 流水灯

- 学习日期：2026-07-14
- MCU：STM32F103C8T6
- 状态：已完成

## 学习目标

- 使LED轮流点亮，达到流水灯的视觉效果

## 核心原理

接线图：





![3-2 LED流水灯.jpg](D:\BaiduNetdiskDownload\程序源码\程序源码\STM32Project-无注释版\1-1%20接线图\3-2%20LED流水灯.jpg)



使A0 A1 ··· ··· A7 轮流输出低电平，从而轮流点亮对应的LED.

### 下面附上源码

```c
#include "stm32f10x.h"                  // Device header
#include "Delay.h"
int main (void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA 
    | RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitTypeDef GPIO_Instructure;
    GPIO_Instructure.GPIO_Mode = GPIO_Mode_Out_PP ;
    GPIO_Instructure.GPIO_Pin = 
     GPIO_Pin_0
    |GPIO_Pin_1
    |GPIO_Pin_2
    |GPIO_Pin_3
    |GPIO_Pin_4
    |GPIO_Pin_5
    |GPIO_Pin_6
    |GPIO_Pin_7;
    GPIO_Instructure.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_Init(GPIOA , &GPIO_Instructure);

    while (1)
    {
    GPIO_Write(GPIOA , ~0x0001);
    Delay_ms(500);
    GPIO_Write(GPIOA , ~0x0002 );
    Delay_ms(500);
    GPIO_Write(GPIOA , ~0x0004 );
    Delay_ms(500);
    GPIO_Write(GPIOA , ~0x0008 );
    Delay_ms(500);
    GPIO_Write(GPIOA , ~0x0010 );
    Delay_ms(500);
    GPIO_Write(GPIOA , ~0x0020 );
    Delay_ms(500);
    GPIO_Write(GPIOA , ~0x00040);
    Delay_ms(500);
    GPIO_Write(GPIOA , ~0x00080);
    Delay_ms(500);
    }
}
```

这里 `~0x0001` 即 1111 1111 1111 1110

~ 是按位取反运算符

此时A0输出低电平，其余输出高电平

感谢[江协科技](https://space.bilibili.com/383400717)的 STM32 入门教程。
