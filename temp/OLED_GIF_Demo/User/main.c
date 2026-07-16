#include "stm32f10x.h"
#include "Delay.h"
#include "OLED.h"
#include "GifFrames.h"

int main(void)
{
    uint16_t FrameIndex = 0U;

    OLED_Init();

    while (1)
    {
        OLED_ShowFrame(OLED_GifFrames[FrameIndex]);
        Delay_ms(OLED_GifDelayMs[FrameIndex]);

        FrameIndex++;

        if (FrameIndex >= OLED_GIF_FRAME_COUNT)
        {
            FrameIndex = 0U;
        }
    }
}
