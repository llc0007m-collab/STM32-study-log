#include "stm32f10x.h"                  // Device header
#include "Delay.h" 

void KEY_Init(void)
	{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_InitTypeDef Key1;
	Key1.GPIO_Mode = GPIO_Mode_IPU;
	Key1.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_11;
	Key1.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&Key1);
	}
	
uint8_t key_GetNum(void)
{
	uint8_t KeyNum = 0;
	if (GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1) == 0)
	{
		Delay_ms(20);
		while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1) == 0);
		Delay_ms(20);
		KeyNum = 1;
	}
	if (GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11) == 0)
	{
		Delay_ms(20);
		while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11) == 0);
		Delay_ms(20);
		KeyNum = 2;
	}
	return KeyNum;
}
	