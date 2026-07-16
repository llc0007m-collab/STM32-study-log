#ifndef __OLED_H
#define __OLED_H

#include "stm32f10x.h"

#define OLED_WIDTH             128U
#define OLED_HEIGHT             64U
#define OLED_FRAME_SIZE       1024U

void OLED_Init(void);
void OLED_Clear(void);
void OLED_ShowFrame(const uint8_t *Frame);

#endif
