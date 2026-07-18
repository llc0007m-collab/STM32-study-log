#include "stm32f10x.h"                  // Device header

void Init_pc13()
	{
	GPIO_InitTypeDef LED1;
	LED1.GPIO_Mode = GPIO_Mode_Out_PP;
	LED1.GPIO_Pin = GPIO_Pin_13;
	LED1.GPIO_Speed = GPIO_Speed_50MHz;
		
	GPIO_Init(GPIOC , &LED1);
	}
void Init_A0A7()
	{
	GPIO_InitTypeDef A0A7;
	A0A7.GPIO_Mode = GPIO_Mode_Out_PP;
	A0A7.GPIO_Pin = 
		GPIO_Pin_0|
		GPIO_Pin_1|
		GPIO_Pin_2|
		GPIO_Pin_3|
		GPIO_Pin_4|
		GPIO_Pin_5|
		GPIO_Pin_6|
		GPIO_Pin_7;
	A0A7.GPIO_Speed = GPIO_Speed_50MHz;
		
	GPIO_Init(GPIOC , &A0A7);
	}