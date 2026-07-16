#ifndef __GIF_FRAMES_H
#define __GIF_FRAMES_H

#include "stm32f10x.h"

/*
 * 128x64 OLED animation.
 * Background pixels are OFF (0), subject pixels are ON (1).
 * Frames are stored with simple RLE compression to reduce Flash usage.
 */
#define OLED_GIF_FRAME_COUNT    34U
#define OLED_GIF_FRAME_SIZE     1024U

extern const uint16_t OLED_GifDelayMs[OLED_GIF_FRAME_COUNT];

/* Decode one frame into a 1024-byte caller-provided buffer. */
void OLED_GifDecodeFrame(uint16_t FrameIndex, uint8_t *FrameBuffer);

#endif
