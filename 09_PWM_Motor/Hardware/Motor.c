#include "stm32f10x.h"                  // Device header
#include "PWM.h"

int8_t speed = 0;

void Motor_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	PWM_Init();
}

void Motor_SetSpeed(int8_t Speed)
{
	if (Speed >= 0)
	{
		GPIO_SetBits(GPIOA, GPIO_Pin_4);
		GPIO_ResetBits(GPIOA, GPIO_Pin_5);
		PWM_SetCompare3(Speed);
	}
	if (Speed < 0)
	{
		GPIO_SetBits(GPIOA, GPIO_Pin_5);
		GPIO_ResetBits(GPIOA, GPIO_Pin_4);
		PWM_SetCompare3(-Speed);
	}
	speed = Speed;
}
void SpeedUp(void)
{
	if (speed >= 100)
	{
		Motor_SetSpeed(100);
	}
	else
	{
	Motor_SetSpeed(speed + 10);
	}
}

void SlowDown(void)
{
	if (speed <= -100)
	{
		Motor_SetSpeed(-100);
	}
	else
	{
	Motor_SetSpeed(speed - 10);
	}
}