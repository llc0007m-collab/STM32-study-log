#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "LED.h"
#include "key.h"

uint8_t Key_Num;

int main (void)
{
	LED_Init();
	KEY_Init();
	while (1)
	{
		Key_Num = key_GetNum();
		if(Key_Num == 1)
			{
			LED1_Open();
			}
		if(Key_Num == 2)
		{
		LED1_Off();
		}
	}
}
