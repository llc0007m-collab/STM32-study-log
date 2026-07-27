#ifndef PWM_H
#define PWM_H

#include "stm32f10x.h"                  // Device header

void PWM_Init(void);
void PWM_SetCompare3(uint16_t Compare);
void Speed_Up(void);
void Speed_Down(void);
void Motor_Init(void);

#endif
