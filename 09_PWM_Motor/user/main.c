#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Motor.h"
#include "key.h"

extern int8_t speed;

int main (void)
{
	Motor_Init();
	OLED_Init();
	KEY_Init();
	
while (1)
{
	OLED_ShowSignedNum(1,1,speed,3);
}
}
