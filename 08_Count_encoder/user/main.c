#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "Init.h"
#include "Blink0-7.h"
#include "OLED.h"
#include "Countsensor.h"
#include "Timer.h"

int16_t  Num;

int main()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC , ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);

	Init_pc13();
	OLED_Init();
	Init_Count();
	Init_B14();
	Timer_Init();
	
	GPIO_WriteBit(GPIOB, GPIO_Pin_14, Bit_SET);
	
	OLED_ShowString(2,3,"Num = ");
	OLED_ShowString(3,3,"Counter=");
	while(1)
	{
		OLED_ShowNum(2,9,Num,5);
		OLED_ShowNum(3,9,TIM_GetCounter(TIM2),5);
	}
}