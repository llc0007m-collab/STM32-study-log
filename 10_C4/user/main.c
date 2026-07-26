#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "CountDown.h"
#include "key.h"
#include "Buzzer.h"
#include "Delay.h"
#include "LED.h"

extern uint16_t Time;
extern uint8_t s ;
extern uint16_t ms ;


int main(void)
{
	OLED_Init();
	Buzzer_Init();
	LED1_Init();
	CountDown_Init();
	AcCountDown_Init();
	
	OLED_ShowString(2,3,"Time=");
	OLED_ShowString(2,10,":");
	
    while (1)
    {
		OLED_ShowNum(2,8,s,2);
		OLED_ShowNum(2,11,ms,3);
		
		if (Time <= 0)
		{
			while(1)
			{
				OLED_Clear();
				Delay_ms(500);
				OLED_ShowString(3,8,"BOOM!");
				OLED_ShowString(2,3,"Time=00:000");
				Delay_ms(500);
			}
		}
    }
}
