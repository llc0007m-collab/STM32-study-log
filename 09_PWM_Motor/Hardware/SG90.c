#include "stm32f10x.h"                  // Device header
#include "PWM.h"

#define SERVO_MAX_ANGLE 180.0f
#define SERVO_MIN_ANGLE 0.0f

float angle = 0.0f;

void Servo_Init(void)
{
	PWM_Init();
}

void Angle_set(float Angle)
{
	if (Angle > 180.0)
	{
		Angle = 180.0f;
	}
	
	if(Angle < 0)
	{
		Angle = 0.0f;
	}
	
	PWM_SetCompare3(2000.0f / 180.0f * Angle + 500.0f);
	angle = Angle;
}



void Increase_Angle(void)
{
	Angle_set(angle + 30.0f);
}

void Decrease_Angle(void)
{
	Angle_set(angle - 30.0f);
}