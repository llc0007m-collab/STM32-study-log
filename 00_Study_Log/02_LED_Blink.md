# 02_LED_Blink 闪烁

- 学习日期：2026-07-14
- MCU：STM32F103C8T6
- 状态：已完成

## 学习目标

使A0针脚外接的LED闪烁





![3-1 LED闪烁.jpg](D:\BaiduNetdiskDownload\程序源码\程序源码\STM32Project-无注释版\1-1%20接线图\3-1%20LED闪烁.jpg)



## 核心原理

> 使用GPIOA之前必须使用 `RCC` 开启GPIOA的外设时钟
> 
> A0需要被设置为推挽输出(GPIO_Mode_Out_PP)
> 
> A0输出高电平时，LED熄灭; A0输出低电平时，LED亮起

## 下面附上源码 `main.c`

```c
#include "stm32f10x.h"                  // Device header
#include "Delay.h"
int main (void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitTypeDef GPIO_Instructure;
    GPIO_Instructure.GPIO_Mode = GPIO_Mode_Out_PP ;
    GPIO_Instructure.GPIO_Pin = GPIO_Pin_0 ;
    GPIO_Instructure.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_Init(GPIOA , &GPIO_Instructure);



    while (1)
    {
    GPIO_WriteBit(GPIOA , GPIO_Pin_0 , Bit_RESET);
    Delay_ms(5000);
    GPIO_WriteBit(GPIOA , GPIO_Pin_0 , Bit_SET);
    Delay_ms (5000);
    }
}
```

**注意，这里`Delay.h`为第三方函数，并非标准库**

下面附上`Delay.h` 与`Delay.c`

我还没有认真研读此函数，留至后日再着手

```c
#include "stm32f10x.h"

/**
  * @brief  微秒级延时
  * @param  xus 延时时长，范围：0~233015
  * @retval 无
  */
void Delay_us(uint32_t xus)
{
    SysTick->LOAD = 72 * xus;                //设置定时器重装值
    SysTick->VAL = 0x00;                    //清空当前计数值
    SysTick->CTRL = 0x00000005;                //设置时钟源为HCLK，启动定时器
    while(!(SysTick->CTRL & 0x00010000));    //等待计数到0
    SysTick->CTRL = 0x00000004;                //关闭定时器
}

/**
  * @brief  毫秒级延时
  * @param  xms 延时时长，范围：0~4294967295
  * @retval 无
  */
void Delay_ms(uint32_t xms)
{
    while(xms--)
    {
        Delay_us(1000);
    }
}

/**
  * @brief  秒级延时
  * @param  xs 延时时长，范围：0~4294967295
  * @retval 无
  */
void Delay_s(uint32_t xs)
{
    while(xs--)
    {
        Delay_ms(1000);
    }
} 
```

```c
#ifndef __DELAY_H
#define __DELAY_H
#include "stm32f10x.h"

void Delay_us(uint32_t us);
void Delay_ms(uint32_t ms);
void Delay_s(uint32_t s);

#endif
```

感谢[江协科技](https://space.bilibili.com/383400717)的 STM32 入门教程。
