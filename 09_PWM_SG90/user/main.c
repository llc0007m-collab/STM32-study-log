#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "SG90.h"
#include "key.h"

extern float angle;

int main (void)
{
	OLED_Init();
	Servo_Init();
	KEY_Init();
	
	
while (1)
{
	OLED_ShowNum(1,1,angle,3);
}
}
