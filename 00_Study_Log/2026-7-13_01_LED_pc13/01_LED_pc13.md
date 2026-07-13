# 01_LED_pc13

## 点亮pc13 led

这是我学习STM32的第一次尝试，跟随[江协科技 # STM32入门教程-2023版](https://www.bilibili.com/video/BV1th411z7sn/?p=4&spm_id_from=333.1007.top_right_bar_window_history.content.click&vd_source=de1141d26d96f000f44e785efa89f5a9)的学习，学会了如何新建工程以及标准库的初步使用，并成功地点亮了pc13 led。

### 新建工程步骤

> 1.建立工程文件夹，Keil中新建工程，选择型号
> 2.工程文件夹里建立Start、Library、User等文件夹，复制固件库里面
> 的文件到工程文件夹
> 3.工程里对应建立Start、Library、User等同名称的分组，然后将对应文件添加到相应分组中
> 4.工程选项 → C/C++ → Include Paths内声明所有包含头文件的文件夹
> 5.工程选项 → Define内定USE_STDPERIPH_DRIVER
> 
> 6.工程选项 → Debug，下拉列表选择对应调试器，Settings → Flash Download里勾选Reset and Run

### **下面附上main.c源码**

```c
#include "stm32f10x.h"                  // Device header

int main(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC,&GPIO_InitStructure);
    GPIO_SetBits(GPIOC, GPIO_Pin_13);        //pc13输出高电平
    //GPIO_ResetBits(GPIOC,GPIO_Pin_13);    // pc13输出低电平
    while (1)
    {

    }
}
```

注：最后一行留出空白行，避免出现编译器警告 。

感谢[江协科技](https://space.bilibili.com/383400717)的讲解
