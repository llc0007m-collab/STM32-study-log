#include "GifFrames.h"

/*
 * Each frame is 1024 bytes in SSD1306 page order.
 * The bytes are RLE-compressed, then Base64-encoded.
 * Data is split into .inc files only to keep this generated source readable.
 */

static const uint16_t OLED_GifFrameOffset[OLED_GIF_FRAME_COUNT + 1U] =
{
    0U, 424U, 850U, 1250U, 1660U, 2070U, 2440U, 2832U, 3244U, 3674U, 4098U, 4526U,
    4910U, 5298U, 5704U, 6130U, 6550U, 6946U, 7356U, 7728U, 8100U, 8490U, 8924U, 9356U,
    9776U, 10208U, 10588U, 10978U, 11388U, 11810U, 12236U, 12654U, 13078U, 13484U, 13888U,
};

static const char OLED_GifRleBase64[] =
#include "GifData00.inc"
#include "GifData01.inc"
#include "GifData02.inc"
#include "GifData03.inc"
#include "GifData04.inc"
;

const uint16_t OLED_GifDelayMs[OLED_GIF_FRAME_COUNT] =
{
    100U, 100U, 100U, 100U, 100U, 100U, 100U, 100U, 100U, 100U, 100U, 100U, 100U, 100U, 100U, 100U,
    100U, 100U, 100U, 100U, 100U, 100U, 100U, 100U, 100U, 100U, 100U, 100U, 100U, 100U, 100U, 100U,
    100U, 100U,
};

static uint8_t OLED_Base64Value(char Character)
{
    if ((Character >= 'A') && (Character <= 'Z'))
    {
        return (uint8_t)(Character - 'A');
    }

    if ((Character >= 'a') && (Character <= 'z'))
    {
        return (uint8_t)(Character - 'a' + 26);
    }

    if ((Character >= '0') && (Character <= '9'))
    {
        return (uint8_t)(Character - '0' + 52);
    }

    if (Character == '+')
    {
        return 62U;
    }

    if (Character == '/')
    {
        return 63U;
    }

    return 0U;
}

static uint8_t OLED_GifReadRleByte(uint16_t ByteIndex)
{
    uint16_t GroupIndex = (uint16_t)(ByteIndex / 3U);
    uint8_t ByteInGroup = (uint8_t)(ByteIndex % 3U);
    uint16_t TextIndex = (uint16_t)(GroupIndex * 4U);
    uint8_t Value0 = OLED_Base64Value(OLED_GifRleBase64[TextIndex]);
    uint8_t Value1 = OLED_Base64Value(OLED_GifRleBase64[TextIndex + 1U]);
    uint8_t Value2 = OLED_Base64Value(OLED_GifRleBase64[TextIndex + 2U]);
    uint8_t Value3 = OLED_Base64Value(OLED_GifRleBase64[TextIndex + 3U]);

    if (ByteInGroup == 0U)
    {
        return (uint8_t)((Value0 << 2U) | (Value1 >> 4U));
    }

    if (ByteInGroup == 1U)
    {
        return (uint8_t)((Value1 << 4U) | (Value2 >> 2U));
    }

    return (uint8_t)((Value2 << 6U) | Value3);
}

void OLED_GifDecodeFrame(uint16_t FrameIndex, uint8_t *FrameBuffer)
{
    uint16_t ReadIndex;
    uint16_t EndIndex;
    uint16_t WriteIndex = 0U;

    if ((FrameBuffer == 0) || (FrameIndex >= OLED_GIF_FRAME_COUNT))
    {
        return;
    }

    ReadIndex = OLED_GifFrameOffset[FrameIndex];
    EndIndex = OLED_GifFrameOffset[FrameIndex + 1U];

    while (((uint16_t)(ReadIndex + 1U) < EndIndex) &&
           (WriteIndex < OLED_GIF_FRAME_SIZE))
    {
        uint8_t RunLength = OLED_GifReadRleByte(ReadIndex);
        uint8_t Value = OLED_GifReadRleByte((uint16_t)(ReadIndex + 1U));

        ReadIndex = (uint16_t)(ReadIndex + 2U);

        while ((RunLength > 0U) &&
               (WriteIndex < OLED_GIF_FRAME_SIZE))
        {
            FrameBuffer[WriteIndex] = Value;
            WriteIndex++;
            RunLength--;
        }
    }

    while (WriteIndex < OLED_GIF_FRAME_SIZE)
    {
        FrameBuffer[WriteIndex] = 0U;
        WriteIndex++;
    }
}
