#include "stm32f10x.h"                  // Device header
#include "Delay.h" 
#include "SG90.h"

extern int Angle;

void KEY_Init(void)
	{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14| GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);	
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource14);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource15);
	
	EXTI_InitTypeDef EXTI_InitStruct ;
	EXTI_InitStruct.EXTI_Line = EXTI_Line14| EXTI_Line15;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;

	EXTI_Init(&EXTI_InitStruct);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStruct);
	}	

void key_IncreaseAngle(void)
{
	if (GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14) == 0)
	{
		Delay_ms(20);
		while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14) == 0);
		Delay_ms(20);
		Increase_Angle();
	}
}

void key_DecreaseAngle(void)
{
	if (GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_15) == 0)
	{
		Delay_ms(20);
		while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_15) == 0);
		Delay_ms(20);
		Decrease_Angle();
	}
}

void EXTI15_10_IRQHandler(void)
{
    if (EXTI_GetITStatus(EXTI_Line14) == SET)
    {
		EXTI_ClearITPendingBit(EXTI_Line14);
		key_IncreaseAngle();
    }
    if (EXTI_GetITStatus(EXTI_Line15) == SET)
    {
		EXTI_ClearITPendingBit(EXTI_Line15);
		key_DecreaseAngle();
    }
}