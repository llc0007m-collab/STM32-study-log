#include "stm32f10x.h"                  // Device header
#include "LED.h"
#include "Timer.h"
#include "Motor.h"
#include "Key.h"
#include "OLED.h"

extern int speed;

int main(void)
{
	LED13_Init();
	PWM_Init();
	Motor_Init();
	Key_Init();
	OLED_Init();
	
	TIM3_Init();
	while(1)
	{
		OLED_ShowSignedNum(1,1,speed,3);
	}
}