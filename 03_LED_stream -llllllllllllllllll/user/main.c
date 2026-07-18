#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "Init.h"
#include "Blink0-7.h"

int main()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC , ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);

	Init_pc13();
	Init_A0A7();
		
	while(1)
	{
	GPIO_SetBits(GPIOC, GPIO_Pin_13);
	Delay_ms(500);
	Blink07();
	GPIO_ResetBits(GPIOC, GPIO_Pin_13);
	Delay_ms(500);
	}
}