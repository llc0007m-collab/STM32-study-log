#include "stm32f10x.h"
#include "Delay.h"
#include "OLED.h"
#include "GifFrames.h"

int main(void)
{
    uint16_t FrameIndex = 0U;
    static uint8_t FrameBuffer[OLED_GIF_FRAME_SIZE];

    OLED_Init();

    while (1)
    {
        OLED_GifDecodeFrame(FrameIndex, FrameBuffer);
        OLED_ShowFrame(FrameBuffer);
        Delay_ms(OLED_GifDelayMs[FrameIndex]);

        FrameIndex++;

        if (FrameIndex >= OLED_GIF_FRAME_COUNT)
        {
            FrameIndex = 0U;
        }
    }
}
