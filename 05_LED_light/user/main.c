#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "LED.h"
#include "key.h"
#include "buzzer.h"
#include "LightSensor.h"

uint8_t Key_Num;

int main (void)
{
	KEY_Init();
	Buzzer_Init();
	LED_Init();
	LSR_Init();
	while (1)
	{
		if(LSR_Get() == 1)
		{
		Buzzer_Open();
		LED_Turn ();
		}
		else
		{
		Buzzer_Off();
		}
	}
}
