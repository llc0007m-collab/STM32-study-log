#include "stm32f10x.h"                  // Device header

void LSR_Init(void)
	{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_InitTypeDef Key1;
	Key1.GPIO_Mode = GPIO_Mode_IPU;
	Key1.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_11;
	Key1.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&Key1);
	}
	
uint8_t LSR_Get(void)
{
	return GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_13);
}
