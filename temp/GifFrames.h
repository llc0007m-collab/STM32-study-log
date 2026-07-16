#ifndef __GIF_FRAMES_H
#define __GIF_FRAMES_H

#include "stm32f10x.h"

#define OLED_GIF_FRAME_COUNT    34U
#define OLED_GIF_FRAME_SIZE     1024U

extern const uint8_t OLED_GifFrames
    [OLED_GIF_FRAME_COUNT][OLED_GIF_FRAME_SIZE];

extern const uint16_t OLED_GifDelayMs
    [OLED_GIF_FRAME_COUNT];

#endif
