#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "Delay.h"
#include "Buzzer.h"
#include "LED.h"

#define BEEP_MAX_INTERVAL_MS  1000
#define BEEP_MIN_INTERVAL_MS   30

uint16_t Time = 30000;
uint8_t s;
uint16_t ms;

void CountDown_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	/* 配置定时器时基 */
	TIM_TimeBaseInitTypeDef TIM_TimBaseInitStructure;
	TIM_TimBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimBaseInitStructure.TIM_Period = 10 - 1;
	TIM_TimBaseInitStructure.TIM_Prescaler = 7200 - 1;
	
	TIM_TimeBaseInit(TIM2, &TIM_TimBaseInitStructure);
	
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);			//清除初始化时可能产生的更新标志
	
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);			//开启定时器内部更新源
	
	NVIC_InitTypeDef NVIC_InitStructure;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	/* 配置 NVIC */
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_Cmd(TIM2, ENABLE);								//启动定时器
}

void AcCountDown_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	
	/* 配置定时器时基 */
	TIM_TimeBaseInitTypeDef TIM_TimBaseInitStructure;
	TIM_TimBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimBaseInitStructure.TIM_Period = BEEP_MAX_INTERVAL_MS * 10 - 1;
	TIM_TimBaseInitStructure.TIM_Prescaler = 7200 - 1;
	
	TIM_TimeBaseInit(TIM3, &TIM_TimBaseInitStructure);
	
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);			//清除初始化时可能产生的更新标志
	
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);			//开启定时器内部更新源
	
	NVIC_InitTypeDef NVIC_InitStructure;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	/* 配置 NVIC */
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_Cmd(TIM3, ENABLE);								//启动定时器
}


void Time_s(void)
{
	s = Time / 1000;
	ms = Time % 1000;
}

void Time_show(void)
{

	{
		if(Time > 0)
		{ 
		Time --;
		Time_s();
		}
		if(Time == 0) 
		{
            TIM_ITConfig(TIM2, TIM_IT_Update, DISABLE);
			TIM_Cmd(TIM2, DISABLE);
			TIM_Cmd(TIM3, DISABLE);
			GPIO_SetBits(GPIOA, GPIO_Pin_2);
		}
	}
}
	

/* 编写中断函数 */
void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
		Time_show();
	}
}

void TIM3_IRQHandler(void)
{
    uint16_t remaining;
    uint16_t interval_ms;

    if (TIM_GetITStatus(TIM3, TIM_IT_Update) == SET)
    {
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);

        Buzzer_Turn();
		LED_Turn();

        remaining = Time;

        interval_ms =
            BEEP_MIN_INTERVAL_MS
            + (uint32_t)remaining
              * (BEEP_MAX_INTERVAL_MS - BEEP_MIN_INTERVAL_MS)
              / 30000;

        TIM_SetAutoreload(
            TIM3,
            interval_ms * 10 - 1
        );
    }
}