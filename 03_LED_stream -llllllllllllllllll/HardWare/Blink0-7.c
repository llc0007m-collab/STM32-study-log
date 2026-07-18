#include "stm32f10x.h"                  // Device header
#include "Delay.h"

#define All_Pins (GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|\
					GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7)

void Blink07()
{
	{
	static uint8_t step = 0;
	uint16_t Pin;
    Pin = (uint16_t)(1U << step);
		GPIO_SetBits(GPIOA,All_Pins);
		GPIO_ResetBits(GPIOA, Pin);
		step++;
	}
}