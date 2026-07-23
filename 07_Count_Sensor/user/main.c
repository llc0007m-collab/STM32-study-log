#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "Init.h"
#include "Blink0-7.h"
#include "OLED.h"
#include "Countsensor.h"

int main()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC , ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);

	Init_pc13();
	Init_A0A7();
	OLED_Init();
	Init_Count();
	
	OLED_ShowString(2,3,"Count = ");
	while(1)
	{
	OLED_ShowNum(2,11,count,3);
	GPIO_SetBits(GPIOC, GPIO_Pin_13);
	Delay_ms(500);
	GPIO_ResetBits(GPIOC, GPIO_Pin_13);
	Delay_ms(300);
	Blink07();
	Delay_ms(200);
	}
}