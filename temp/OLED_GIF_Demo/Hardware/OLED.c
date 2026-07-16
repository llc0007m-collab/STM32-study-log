#include "stm32f10x.h"
#include "Delay.h"
#include "OLED.h"

/*
 * 已确认的软件I2C引脚：
 * PB8  -> OLED SCL
 * PB9  -> OLED SDA
 *
 * 这里把PB8/PB9当作普通开漏GPIO使用，并未启用硬件I2C1，
 * 因此不需要I2C1外设时钟，也不需要AFIO重映射。
 */
#define OLED_GPIO_PORT         GPIOB
#define OLED_GPIO_CLK          RCC_APB2Periph_GPIOB
#define OLED_SCL_PIN           GPIO_Pin_8
#define OLED_SDA_PIN           GPIO_Pin_9

/* OLED常见7位地址0x3C，对应写地址0x78 */
#define OLED_WRITE_ADDRESS     0x78U

#define OLED_W_SCL(x) \
    GPIO_WriteBit(OLED_GPIO_PORT, OLED_SCL_PIN, (BitAction)(x))

#define OLED_W_SDA(x) \
    GPIO_WriteBit(OLED_GPIO_PORT, OLED_SDA_PIN, (BitAction)(x))

static void OLED_I2C_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(OLED_GPIO_CLK, ENABLE);

    GPIO_InitStructure.GPIO_Pin = OLED_SCL_PIN | OLED_SDA_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_Init(OLED_GPIO_PORT, &GPIO_InitStructure);

    /* 开漏输出写1表示释放总线，由上拉电阻拉高 */
    OLED_W_SCL(1);
    OLED_W_SDA(1);
}

static void OLED_I2C_Start(void)
{
    OLED_W_SDA(1);
    OLED_W_SCL(1);
    OLED_W_SDA(0);
    OLED_W_SCL(0);
}

static void OLED_I2C_Stop(void)
{
    OLED_W_SDA(0);
    OLED_W_SCL(1);
    OLED_W_SDA(1);
}

static void OLED_I2C_SendByte(uint8_t Byte)
{
    uint8_t BitIndex;

    for (BitIndex = 0U; BitIndex < 8U; BitIndex++)
    {
        OLED_W_SDA((Byte & (uint8_t)(0x80U >> BitIndex)) != 0U);
        OLED_W_SCL(1);
        OLED_W_SCL(0);
    }

    /*
     * 发送第9个时钟。
     * 本驱动保持与原工程一致，不读取ACK。
     */
    OLED_W_SCL(1);
    OLED_W_SCL(0);
}

static void OLED_WriteCommand(uint8_t Command)
{
    OLED_I2C_Start();
    OLED_I2C_SendByte(OLED_WRITE_ADDRESS);
    OLED_I2C_SendByte(0x00U);
    OLED_I2C_SendByte(Command);
    OLED_I2C_Stop();
}

static void OLED_WriteDataArray(const uint8_t *Data, uint16_t Count)
{
    OLED_I2C_Start();
    OLED_I2C_SendByte(OLED_WRITE_ADDRESS);
    OLED_I2C_SendByte(0x40U);

    while (Count > 0U)
    {
        OLED_I2C_SendByte(*Data);
        Data++;
        Count--;
    }

    OLED_I2C_Stop();
}

static void OLED_SetCursor(uint8_t Page, uint8_t Column)
{
    OLED_WriteCommand((uint8_t)(0xB0U | Page));
    OLED_WriteCommand((uint8_t)(0x10U | ((Column & 0xF0U) >> 4U)));
    OLED_WriteCommand((uint8_t)(0x00U | (Column & 0x0FU)));
}

void OLED_Clear(void)
{
    uint8_t Page;
    static const uint8_t EmptyLine[OLED_WIDTH] = {0};

    for (Page = 0U; Page < 8U; Page++)
    {
        OLED_SetCursor(Page, 0U);
        OLED_WriteDataArray(EmptyLine, OLED_WIDTH);
    }
}

void OLED_ShowFrame(const uint8_t *Frame)
{
    uint8_t Page;

    if (Frame == 0)
    {
        return;
    }

    for (Page = 0U; Page < 8U; Page++)
    {
        OLED_SetCursor(Page, 0U);
        OLED_WriteDataArray(
            &Frame[(uint16_t)Page * OLED_WIDTH],
            OLED_WIDTH
        );
    }
}

void OLED_Init(void)
{
    /* OLED上电后等待电源和控制器稳定 */
    Delay_ms(100U);

    OLED_I2C_Init();

    OLED_WriteCommand(0xAEU);  /* 关闭显示 */

    OLED_WriteCommand(0xD5U);  /* 显示时钟分频 */
    OLED_WriteCommand(0x80U);

    OLED_WriteCommand(0xA8U);  /* 多路复用率：64行 */
    OLED_WriteCommand(0x3FU);

    OLED_WriteCommand(0xD3U);  /* 显示偏移 */
    OLED_WriteCommand(0x00U);

    OLED_WriteCommand(0x40U);  /* 显示起始行 */

    OLED_WriteCommand(0xA1U);  /* 段重映射 */
    OLED_WriteCommand(0xC8U);  /* COM扫描方向 */

    OLED_WriteCommand(0xDAU);  /* COM引脚配置 */
    OLED_WriteCommand(0x12U);

    OLED_WriteCommand(0x81U);  /* 对比度 */
    OLED_WriteCommand(0xCFU);

    OLED_WriteCommand(0xD9U);  /* 预充电周期 */
    OLED_WriteCommand(0xF1U);

    OLED_WriteCommand(0xDBU);  /* VCOMH */
    OLED_WriteCommand(0x30U);

    OLED_WriteCommand(0xA4U);  /* 使用显存内容 */
    OLED_WriteCommand(0xA6U);  /* 正常显示 */

    OLED_WriteCommand(0x8DU);  /* 充电泵 */
    OLED_WriteCommand(0x14U);

    OLED_WriteCommand(0xAFU);  /* 开启显示 */

    OLED_Clear();
}
