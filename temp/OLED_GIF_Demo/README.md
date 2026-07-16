# OLED GIF 临时实验工程

## 实验目标

使用 STM32F103C8T6、STM32F10x SPL 和 128×64 单色 OLED，循环播放 `temp/GifFrames.c` 中的 34 帧动画。

## 工程文件

打开：

```text
temp/OLED_GIF_Demo/OLED_GIF_Demo.uvprojx
```

该临时工程复用仓库中已有的 SPL、启动和延时文件：

```text
06_LED_OLED/Start
06_LED_OLED/Library
06_LED_OLED/System
```

动画数据仍放在：

```text
temp/GifFrames.c
temp/GifFrames.h
```

因此不要只把 `OLED_GIF_Demo` 文件夹单独移出仓库，否则相对路径会失效。

## 已确认的工程配置

- 芯片：STM32F103C8T6
- 器件宏：`STM32F10X_MD`
- SPL 宏：`USE_STDPERIPH_DRIVER`
- 启动文件：`startup_stm32f10x_md.s`
- 系统时钟：沿用现有 `system_stm32f10x.c`，按外部 8 MHz 晶振配置为 72 MHz
- OLED 通信方式：软件模拟 I2C
- OLED 写地址：`0x78`，对应常见 7 位地址 `0x3C`
- 动画：34 帧，每帧 1024 字节，每帧延时 100 ms

## 引脚确认

| OLED 模块丝印 | STM32 引脚 | LQFP48 芯片脚号 | 本工程用途 |
|---|---:|---:|---|
| GND | GND | — | 必须共地 |
| VCC | 3.3V | — | 首次实验优先使用 3.3V |
| SCL / SCK | PB8 | 45 | 软件 I2C 时钟 |
| SDA | PB9 | 46 | 软件 I2C 数据 |

PB8、PB9 都是有效 GPIO。它们也可以作为重映射后的硬件 I2C1 引脚，但本工程只把它们配置成 `GPIO_Mode_Out_OD`，没有启用 I2C1，也没有执行 AFIO 重映射。

## 仍需实物确认

- OLED 模块接口丝印是否确实为 `GND/VCC/SCL/SDA`
- 控制器是否兼容 SSD1306 初始化命令
- 7 位地址是否为 `0x3C`
- 模块板上是否已有 SCL、SDA 上拉电阻
- 模块允许的供电电压

不能只凭商品宣传图确认以上信息。若原工程已经能显示字符 `A`，则控制器、地址和基本接线大概率已经正确。

## 编译和下载

1. 在 Keil uVision5 打开 `OLED_GIF_Demo.uvprojx`。
2. 执行 Rebuild。
3. 检查编译结果中的 `Code + RO-data` 总量是否小于 64 KB。
4. 连接 ST-Link，确认 SWDIO、SWCLK、GND。
5. 下载后复位。

34 帧原始图像数据占用：

```text
34 × 1024 = 34816 字节
```

STM32F103C8T6 官方 Flash 容量为 64 KB，因此剩余空间有限。若链接器报告空间不足，应减少 GIF 帧数，而不是把帧数组去掉 `const`。

## 成功判据

OLED 连续循环播放 34 帧动画，换帧时不出现每帧先全黑一次的明显闪烁。

## 最可能的三个问题

1. **整个屏幕不亮**：先检查供电、共地、PB8/PB9 接线和模块地址。
2. **画面黑白相反**：重新转换 GIF 时反转阈值判断。
3. **图像上下或左右颠倒**：调整 OLED 初始化中的 `0xA1/0xA0` 和 `0xC8/0xC0`。
