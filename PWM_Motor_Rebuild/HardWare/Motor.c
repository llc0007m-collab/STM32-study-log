#include "stm32f10x.h"                  // Device header

int speed;

void PWM_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	TIM_InternalClockConfig(TIM2);

	TIM_TimeBaseInitTypeDef TimeBaseInitStructure;
	TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TimeBaseInitStructure.TIM_Period = 100 -1;
	TimeBaseInitStructure.TIM_Prescaler = 36 -1;
	TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2, &TimeBaseInitStructure);
	
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCStructInit(&TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;
	
	TIM_OC3Init(TIM2, &TIM_OCInitStructure);
	
	TIM_Cmd(TIM2, ENABLE);
}

void Motor_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4| GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA ,GPIO_Pin_4);
	GPIO_ResetBits(GPIOA ,GPIO_Pin_5);
}

void Motor_Turn(void)
{
	if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4) == SET &&
		GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5) == RESET)
	{	
	GPIO_ResetBits(GPIOA ,GPIO_Pin_4);
	GPIO_SetBits(GPIOA ,GPIO_Pin_5);
	}
	else
	{
	GPIO_SetBits(GPIOA ,GPIO_Pin_4);
	GPIO_ResetBits(GPIOA ,GPIO_Pin_5);	
	}
}



void PWM_SetCompare3(uint16_t Compare)
{
	TIM_SetCompare3(TIM2, Compare);
}

void Speed_Set(int16_t Speed)
{
	if (Speed >= 0)
	{
	GPIO_SetBits(GPIOA ,GPIO_Pin_4);
	GPIO_ResetBits(GPIOA ,GPIO_Pin_5);
	PWM_SetCompare3(Speed);
	} 
	else
	{
	GPIO_ResetBits(GPIOA ,GPIO_Pin_4);
	GPIO_SetBits(GPIOA ,GPIO_Pin_5);
	PWM_SetCompare3(-Speed);
	}
	speed = Speed;
}

void Speed_Up(void)
{
	if(speed >= 100)
	{
		Speed_Set(100);
	}
	else
	{
		Speed_Set(10 + speed);
	}
}

void Speed_Down(void)
{
	if(speed <= -100)
	{
		Speed_Set(-100);
	}
	else
	{
		Speed_Set(speed - 10);
	}
}
