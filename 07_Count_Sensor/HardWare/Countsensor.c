#include "stm32f10x.h"                  // Device header

void Init_Count(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB| RCC_APB2Periph_AFIO , ENABLE);
	GPIO_InitTypeDef Count;
	Count.GPIO_Mode = GPIO_Mode_IPU;
	Count.GPIO_Pin = GPIO_Pin_14;
	Count.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB,&Count);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource14);
	
	EXTI_InitTypeDef EXTI_I ;
	EXTI_I.EXTI_Line = EXTI_Line14;
	EXTI_I.EXTI_LineCmd = ENABLE;
	EXTI_I.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_I.EXTI_Trigger = EXTI_Trigger_Falling;
	
	EXTI_Init(&EXTI_I);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStruct);
}

volatile  uint32_t count = 0;

void EXTI15_10_IRQHandler(void)
{
    if (EXTI_GetITStatus(EXTI_Line14) == SET)
    {
		if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14) == Bit_RESET)
	{
			count ++;
	}
        EXTI_ClearITPendingBit(EXTI_Line14);
    }
}
