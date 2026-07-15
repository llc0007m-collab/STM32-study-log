#include "stm32f10x.h"                  // Device header
#include "Delay.h"
int main (void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO_Instructure;
	GPIO_Instructure.GPIO_Mode = GPIO_Mode_Out_PP ;
	GPIO_Instructure.GPIO_Pin = 
	 GPIO_Pin_0
	|GPIO_Pin_1
	|GPIO_Pin_2
	|GPIO_Pin_3
	|GPIO_Pin_4
	|GPIO_Pin_5
	|GPIO_Pin_6
	|GPIO_Pin_7;
	GPIO_Instructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_InitTypeDef GPIO_I;
	GPIO_I.GPIO_Mode = GPIO_Mode_Out_PP ;
	GPIO_I.GPIO_Pin = 
	 GPIO_Pin_12;
	GPIO_I.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB , &GPIO_I);
	GPIO_Init(GPIOA , &GPIO_Instructure);	
	while (1)
	{
	GPIO_ResetBits(GPIOB , GPIO_Pin_12);
	Delay_ms(100);
	GPIO_SetBits(GPIOB , GPIO_Pin_12);
	Delay_ms(50);
	GPIO_ResetBits(GPIOB , GPIO_Pin_12);
	Delay_ms(100);
	GPIO_SetBits(GPIOB , GPIO_Pin_12);
	GPIO_Write(GPIOA , ~0x0001);
	Delay_ms(500);
	GPIO_Write(GPIOA , ~0x0002 );
	Delay_ms(500);
	GPIO_Write(GPIOA , ~0x0004 );
	Delay_ms(500);
	GPIO_Write(GPIOA , ~0x0008 );
	Delay_ms(500);
	GPIO_Write(GPIOA , ~0x0010 );
	Delay_ms(500);
	GPIO_Write(GPIOA , ~0x0020 );
	Delay_ms(500);
	GPIO_Write(GPIOA , ~0x00040);
	Delay_ms(500);
	GPIO_Write(GPIOA , ~0x00080);
	Delay_ms(500);
	}
}
