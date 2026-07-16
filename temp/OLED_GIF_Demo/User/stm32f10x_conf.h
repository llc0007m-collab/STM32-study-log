#ifndef __STM32F10x_CONF_H
#define __STM32F10x_CONF_H

/* 本实验只使用GPIO和RCC两个SPL外设驱动。 */
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t *File, uint32_t Line);
#define assert_param(expr) \
    ((expr) ? (void)0 : assert_failed((uint8_t *)__FILE__, __LINE__))
#else
#define assert_param(expr) ((void)0)
#endif

#endif
