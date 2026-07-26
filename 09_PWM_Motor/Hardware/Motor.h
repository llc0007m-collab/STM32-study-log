#ifndef MOTOR_H
#define MOTOR_H

#include "stm32f10x.h"                  // Device header

void Motor_Init(void);
void Motor_SetSpeed(int8_t Speed);
void SpeedUp(void);
void SlowDown(void);

#endif
