#include "stm32f10x.h"                  // Device header

int main(void)
{
    GPIO_InitTypeDef gpio_init = {0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

    gpio_init.GPIO_Mode = GPIO_Mode_Out_PP;
    gpio_init.GPIO_Pin = GPIO_Pin_13;
    gpio_init.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOC, &gpio_init);

    /* This project drives the PC13 LED as active-low. */
    GPIO_ResetBits(GPIOC, GPIO_Pin_13);

    while (1)
    {
    }
}
