#include "stm32f10x.h"                  // Device header

void LED_Init(void)
	{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef LED1;
	LED1.GPIO_Mode = GPIO_Mode_Out_PP;
	LED1.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
	LED1.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&LED1);
		
	GPIO_ResetBits(GPIOA,GPIO_Pin_0 | GPIO_Pin_1);
	}
	
void LED1_Open(void)
	{
	GPIO_SetBits (GPIOA,GPIO_Pin_0);
	}
	
void LED1_Off(void)
{
	GPIO_ResetBits(GPIOA,GPIO_Pin_0);
}

void LED2_Open(void)
	{
	GPIO_SetBits (GPIOA,GPIO_Pin_1);
	}
	
void LED2_Off(void)
{
	GPIO_ResetBits(GPIOA,GPIO_Pin_1);
}