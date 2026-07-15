#include "stm32f10x.h"                  // Device header
#include "Delay.h"
int main (void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_Instructure;
	GPIO_Instructure.GPIO_Mode = GPIO_Mode_Out_PP ;
	GPIO_Instructure.GPIO_Pin = GPIO_Pin_0 ;
	GPIO_Instructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA , &GPIO_Instructure);
	


	while (1)
	{
	GPIO_WriteBit(GPIOA , GPIO_Pin_0 , Bit_RESET);
	Delay_ms(5000);
	GPIO_WriteBit(GPIOA , GPIO_Pin_0 , Bit_SET);
	Delay_ms (5000);
	}
}
