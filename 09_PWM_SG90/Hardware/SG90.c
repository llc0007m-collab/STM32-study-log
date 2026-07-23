#include "stm32f10x.h"                  // Device header
#include "PWM.h"


void Servo_Init(void)
{
	PWM_Init();
}

void Set_Angle(float Angle)
{
	PWM_SetCompare2(2000/180*Angle+500);
}