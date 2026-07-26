#include "stm32f10x.h"                  // Device header

void Buzzer_Init(void)
	{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_InitTypeDef LED1;
	LED1.GPIO_Mode = GPIO_Mode_Out_PP;
	LED1.GPIO_Pin = GPIO_Pin_12;
	LED1.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&LED1);
		
	GPIO_SetBits(GPIOB,GPIO_Pin_12);
	}
	
void Buzzer_Open(void)
	{
	GPIO_ResetBits (GPIOB,GPIO_Pin_12);
	}
	
void Buzzer_Off(void)
{
	GPIO_SetBits(GPIOB,GPIO_Pin_12);
}
