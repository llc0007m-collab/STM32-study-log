#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "LED.h"
#include "key.h"
#include "buzzer.h"

uint8_t Key_Num;

int main (void)
{
	KEY_Init();
	while (1)
	{
		Key_Num = key_GetNum();
		if(Key_Num == 1)
		{
		Buzzer_Open();
		}
		if(Key_Num == 2)
		{
		Buzzer_Off();
		}
	}
}
