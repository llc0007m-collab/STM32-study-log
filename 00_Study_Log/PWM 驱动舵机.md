# PWM 驱动舵机

* 前方疲于学习，停止了学习日志的记录。因为工作量较大，我不计划对前期的缺失进行修补。

* 这篇仅陈述我在这次工程中遇到的问题，以及记录我的解决方案

## 问题1

```c
void Angle_set(float Angle)
{
        /*   ······························   */
	PWM_SetCompare2(2000.0f / 180.0f * Angle + 500.0f);
}
```

**对于此类函数形参如何把输入的值传到函数外供外部使用？**

答：

在此 .c 文件加入全局变量`float angle = 0.0f`，在另外的 .c 中 `extern float angle` ，

从而在别的函数中读取该变量。



## 问题2

**PB14 和 PB15两个端口共用一个`NVIC_IQRChannel`(EXTI15_10_IRQn), 仍然能够同时用于按键触发不同的中断函数吗？**
答：仍然可以

```c
void EXTI15_10_IRQHandler(void)
{
    if (EXTI_GetITStatus(EXTI_Line14) == SET)
    {
        EXTI_ClearITPendingBit(EXTI_Line14);
        /*  PB14 按键触发执行函数  */
    }
    if (EXTI_GetITStatus(EXTI_Line15) == SET)
    {
    	EXTI_ClearITPendingBit(EXTI_Line15);
        /*  PB15 按键触发执行函数  */
    }
}
```

通过 if 条件语句来确认按下的是 B14 还是 B15.






