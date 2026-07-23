# Day 01｜认识 STM32F103 与从零建立 SPL 工程

> 建议用时：7 小时（可在 6～8 小时内微调）  
> 今日范围：视频 P1～P4；PPT 第 1～17 页；基础资源 1-1、1-2、1-3 与工程 2-1  
> 今日出口：不复制现成工程，从空目录建立一个可编译、可下载、可复位运行的 STM32F103C8T6 工程，并能讲清从复位到 `main()` 的调用链。

## 1. 固定环境与规则

- MCU：STM32F103C8T6，ARM Cortex-M3，最高 72 MHz，64 KB Flash，20 KB SRAM，LQFP48，标准 3.3 V 供电。
- 工具：Keil MDK 5.43，ArmClang 6.24，ST-Link/SWD。
- 软件层：CMSIS + **STM32F10x Standard Peripheral Library V3.5.0**（SPL）。
- 固件库唯一来源：`D:\STM32_Library\固件库\固件库\STM32F10x_StdPeriph_Lib_V3.5.0\STM32F10x_StdPeriph_Lib_V3.5.0`。
- 禁止混入：`D:\STM32_Library\STM32F10x_StdPeriph_Lib_V3.6.0`、HAL、LL、CubeMX 生成代码。
- 编译宏至少确认：`STM32F10X_MD`、`USE_STDPERIPH_DRIVER`；启动文件使用 `startup_stm32f10x_md.s`。
- 学习工程只能写入你为今日实验新建的目录；不要直接改课程原始附件。

## 2. 视频、PPT、源码与手册映射

### 2.1 视频（必须完整观看）

课程页：[STM32 入门教程-2023版](https://www.bilibili.com/video/BV1th411z7sn/)

| 顺序 | 视频分集原题 | 时长 | 观看后必须回答 |
|---|---|---:|---|
| P1 | `[1-1] 课程简介` | 11:28 | 课程使用什么板、开发方式、硬件平台和调试工具？ |
| P2 | `[1-2] STM32简介` | 49:02 | STM32、ST、ARM、Cortex-M3 各是什么关系？F103C8T6 名称如何拆解？ |
| P3 | `[2-1] 软件安装` | 16:45 | Keil、器件包、ST-Link 驱动、USB 转串口驱动分别解决什么问题？ |
| P4 | `[2-2] 新建工程` | 42:15 | 从启动文件到 `main()` 的执行链是什么？Include Paths、宏和下载器怎样配置？ |

原速共 1:59:30；允许最高 1.5 倍速，但涉及工程配置、启动链和芯片命名处必须暂停做笔记。

### 2.2 本地材料

| 材料 | 精确路径 | 今日用途 |
|---|---|---|
| 课程 PPT | `D:\STM32_Library\课件PPT.zip`（密码 `32`）内 `课件PPT\STM32入门教程.pptx` 第 1～17 页 | 硬件/软件准备、STM32 与 ARM、型号资源、系统结构、引脚、启动配置、最小系统、工程步骤与架构 |
| 接线图 | `D:\STM32_Library\程序源码\程序源码\STM32Project-有注释版\1-1 接线图` | 对照 SWD、供电和面包板接线 |
| 清理脚本 | `D:\STM32_Library\程序源码\程序源码\STM32Project-有注释版\1-2 keilkill批处理` | 只阅读脚本用途；不要在原附件目录执行 |
| Delay 模块 | `D:\STM32_Library\程序源码\程序源码\STM32Project-有注释版\1-3 Delay函数模块` | 识别 `System` 层；今日只理解接口和 SysTick 依赖 |
| OLED 模块 | `D:\STM32_Library\程序源码\程序源码\STM32Project-有注释版\1-4 OLED驱动函数模块` | 识别可复用硬件驱动层；留待 Day02 使用 |
| 工程模板 | `D:\STM32_Library\程序源码\程序源码\STM32Project-有注释版\2-1 STM32工程模板` | 对照目录和配置，不能作为闭卷任务的复制来源 |
| 数据手册 | `D:\STM32_Library\参考文档\参考文档\STM32F103x8B数据手册（中文）.pdf` | 查器件特性、LQFP48 引脚、电气限制和存储容量 |
| 参考手册 | `D:\STM32_Library\参考文档\参考文档\STM32F10xxx参考手册（中文）.pdf` | 查系统结构、RCC、Flash、GPIO/AFIO 等外设寄存器章节 |
| 内核手册 | `D:\STM32_Library\参考文档\参考文档\STM32F10xxx Cortex-M3编程手册.pdf` | 查异常向量、复位、堆栈与 NVIC 内核概念 |
| SPL 手册 | `D:\STM32_Library\参考文档\参考文档\STM32F103xx固件函数库用户手册.pdf` | 学会从外设找到对应 `.h/.c` 和 SPL API |
| 引脚速查 | `D:\STM32_Library\参考文档\参考文档\STM32F103C8T6引脚定义.png` / `.xlsx` | 交叉核对引脚，禁止凭教程截图猜线 |
| 官方程序修改说明 | `D:\STM32_Library\程序源码\程序源码\程序修改说明.pdf` | 今日没有针对 1-x/2-1 的专属修改；确认后续 6 条勘误的总入口，不虚构“第 7 条” |

## 3. 7 小时时间表

| 时间 | 任务 | 可验收产出 |
|---:|---|---|
| 00:00–00:25 | 保存 `git status --short`、建立今日日志与证据目录；检查板、ST-Link、线材 | 基线状态文本、器材清单 |
| 00:25–02:25 | 看 P1～P4；每 P 写 5～10 条主动回忆笔记 | 分集笔记、暂停点和疑问 |
| 02:25–03:20 | 精读 PPT 1～17；查数据手册中的容量、供电、封装、引脚和 BOOT | 17 页覆盖勾选、命名拆解表 |
| 03:20–04:00 | 阅读接线图、Delay、工程模板及 CMSIS/SPL 目录；画启动链 | 手绘/电子启动链、文件职责表 |
| 04:00–05:45 | 关闭教程，从空目录闭卷建工程、写最小 `main()`、构建 | `.uvprojx`、源码、完整构建日志 |
| 05:45–06:20 | SWD 接线、下载、复位运行；做两项故障注入 | 接线照片、下载记录、排错记录 |
| 06:20–07:00 | Codex 只读验收、逐题口试、复盘与补考安排 | 审查报告或验收对话、当日复盘 |

## 4. 完整知识清单

完成一项才勾选；“看过”不等于“能解释”。

### 4.1 平台与芯片

- [ ] 区分 ST（芯片厂商）、ARM（架构/IP 公司）、Cortex-M3（内核）与 STM32F103（微控制器）。
- [ ] 解释微控制器由内核、总线、存储器、时钟/复位和片上外设组成。
- [ ] 拆解 `STM32F103C8T6`：系列/产品线、引脚数、Flash 档位、封装、温度等级；具体编码以数据手册订购信息表为准。
- [ ] 记住本板硬约束：72 MHz、64 KB Flash、20 KB SRAM、2.0～3.6 V、LQFP48；理解“常见 C8T6 板可能有超规格可见容量”不能成为工程依赖。
- [ ] 能给 NVIC、SysTick、RCC、GPIO、AFIO、EXTI、TIM、ADC、DMA、USART、I2C、SPI、RTC、BKP、PWR、IWDG、WWDG 写出中文作用。
- [ ] 能沿系统结构图说明 Cortex-M3、I-Code/D-Code/System 总线、AHB、APB1、APB2、DMA、Flash、SRAM 和外设间的数据路径。

### 4.2 最小系统与启动

- [ ] 识别供电/去耦、复位、8 MHz HSE、32.768 kHz LSE、BOOT0/BOOT1、SWD 等最小系统组成。
- [ ] 解释 BOOT0/BOOT1（PB2）采样决定从用户 Flash、系统存储器或 SRAM 启动；正常程序运行保持 BOOT0=0。
- [ ] 知道 PA13=SWDIO、PA14=SWCLK，调试阶段不能误作普通 GPIO。
- [ ] 能从复位向量讲到初始 MSP、`Reset_Handler`、`SystemInit()`、C 运行时初始化、`main()`；能说明中断向量表与弱符号默认处理函数的作用。
- [ ] 区分 `.text/.rodata`（通常在 Flash）、`.data`（初值在 Flash、运行时复制到 SRAM）、`.bss`（启动时清零）与栈/堆（SRAM）。
- [ ] 会选择 F103C8T6 的中容量启动文件 `startup_stm32f10x_md.s` 与宏 `STM32F10X_MD`，不会选 `HD/XL/CL/VL`。

### 4.3 工程结构与工具链

- [ ] 能解释 `Start`、`Library`、`System`、`Hardware`、`User` 各层职责，且头文件接口和实现边界清楚。
- [ ] 知道 `core_cm3.*`、`system_stm32f10x.*`、`stm32f10x.h`、`stm32f10x_conf.h`、`misc.*`、外设 `stm32f10x_xxx.*` 各做什么。
- [ ] 能区分器件包、CMSIS、SPL、编译器、链接器、下载器和调试器。
- [ ] 会设置 Device、ArmClang 6.24、Include Paths、`STM32F10X_MD`、`USE_STDPERIPH_DRIVER`、Output、Listing/Map、ST-Link、SW Port、Flash Download、Reset and Run。
- [ ] 理解预处理→编译→汇编→链接→生成 AXF/HEX→下载；能从第一条 Error 开始定位而不是只看最后一行。
- [ ] 会读 `.map`，确认 Flash/RAM 使用量没有超过 64 KB/20 KB。
- [ ] 知道 `keilkill` 是清理生成物的脚本，不应在未知目录或有未保存证据时执行。

### 4.4 必会公式与数量级

- CPU 时钟周期：`T = 1 / f`；72 MHz 时约 `13.89 ns/周期`。
- 二进制容量：`64 KiB = 64 × 1024 B`，`20 KiB = 20 × 1024 B`。
- Flash 典型地址范围按 64 KiB 计算：`0x08000000` 起，到 `0x0800FFFF`；SRAM 按 20 KiB 计算：`0x20000000` 起，到 `0x20004FFF`。
- 资源裕量：`余量 = 器件容量 - map 中实际占用`；不得把链接成功当作运行正确。

## 5. 引脚与接线表

先查板上丝印、原理图和数据手册，再填写“实际连接”；不要按排针物理顺序想当然地直连 ST-Link。

| 信号 | MCU 引脚/板端标号 | 方向 | 电压/默认状态 | 实际连接 | 验证方法 |
|---|---|---|---|---|---|
| 3V3 | 3V3/VDD | 电源输入 | 3.3 V | ______ | 万用表测量 |
| GND | VSS/GND | 电源参考 | 0 V | ______ | 断电通断/上电电压 |
| SWDIO | PA13 | 双向 | 3.3 V 逻辑 | ST-Link SWDIO：______ | Debug Settings 可识别 |
| SWCLK | PA14 | 输入 | 3.3 V 逻辑 | ST-Link SWCLK：______ | Debug Settings 可识别 |
| NRST（可选但推荐） | NRST | 输入 | 上拉，低有效 | ST-Link NRST：______ | 复位/Connect under reset |
| BOOT0 | BOOT0 | 输入 | 正常运行拉低 | ______ | 复位后从用户 Flash 启动 |
| BOOT1 | PB2 | 复用采样 | 由启动模式决定 | ______ | 查板级下拉/程序占用 |

接线前断电；禁止把 5 V 接到 3V3；ST-Link 与目标板必须共地。

## 6. 闭卷编码、实物实验与故障注入

### 6.1 闭卷工程（核心迁移题）

1. 退出/最小化课程工程，从空目录新建工程，目录至少含 `Start`、`Library`、`User`。
2. 只从 SPL V3.5.0 复制必要 CMSIS、启动、系统和 SPL 文件；选择 `startup_stm32f10x_md.s`。
3. 工程中只加入本任务需要的源文件；不要把整个库的所有 `.c` 无脑加入。
4. 配置 `STM32F10X_MD,USE_STDPERIPH_DRIVER`、Include Paths、ArmClang 6.24、ST-Link/SW、Reset and Run。
5. 编写最小 `main()`：完成 `SystemInit()` 之后进入可观察的稳定主循环；可读取 `SystemCoreClock` 或用调试器观察主循环计数变量。今日不要求复制 LED 例程。
6. Build，保存完整日志和 `.map`；必须 `0 Error / 0 Warning`，并记录 Code/RO/RW/ZI 或等价 Flash/RAM 数据。
7. 下载后复位，证明 CPU 到达并持续执行 `main()`；截图必须能看到工程名、时间或调试状态，不能用旧图。
8. 在日志中逐步写出从复位到 `main()` 的调用链，并注明各文件位置。

### 6.2 故障注入（至少做两项并恢复）

- 删除 `USE_STDPERIPH_DRIVER`：预测后再构建，记录第一条错误/行为，恢复宏。
- 将启动文件故意换成错误密度型号：只做构建/链接观察，不下载；说明为什么不能保留。
- 暂时移除一个 Include Path：记录编译器报错如何指向缺失头文件，再恢复。
- 将 Debug Port 设为 JTAG：观察与 FAQ 11 对应的现象后恢复 SW；若有失联风险，不实际下载。
- 断开 SWDIO 或 SWCLK 后尝试识别：记录 `No target connected`，断电后恢复接线。

## 7. 官方 FAQ 与今日排障卡

总索引：[江协科技 STM32 疑难解答](https://jiangxiekeji.com/problem.html)。今日覆盖问题 1～4、11～14。

官方《程序修改说明》的 6 条修改分别针对 5-1、5-2、6-2、10-1、11-1 和 OLED 驱动；Day01 没有专属源码勘误，工程模板以有注释版和本计划锁定的 ArmClang 6.24/V3.5.0 规则验收。

| FAQ | 先查什么 | 今日应掌握的处理顺序 |
|---|---|---|
| [1 `No ST-LINK detected`](https://jiangxiekeji.com/problem/p1-1.html) | 电脑是否识别调试器 | 插拔/换 USB → 设备管理器 → 驱动/黄色叹号 → 替换调试器 |
| [2 `No target connected`](https://jiangxiekeji.com/problem/p1-2.html) | SWD 线序、共地、接触、目标供电 | 按丝印逐根核对 → 换线/测 3V3 → 更新或替换 ST-Link → 必要时按住复位并连接 |
| [3 `Could not load ... .axf`](https://jiangxiekeji.com/problem/p1-3.html) | 是否先成功 Build | 先解决第一条编译/链接错误，确认产生 AXF，再下载 |
| [4 板插不进面包板](https://jiangxiekeji.com/problem/p1-4.html) | 新面包板弹片紧、板脚多 | 对准一边，两侧交替均匀施力；拔出也两侧交替，避免弯针 |
| [11 `Unknown target connected`](https://jiangxiekeji.com/problem/p1-11.html) | Debug 端口是否误选 JTAG | `Options → Debug → Settings → Port` 设为 `SW` |
| [12 文件末尾无换行警告](https://jiangxiekeji.com/problem/p1-12.html) | 最后一行是否真正以换行结束 | 文件末尾补换行，不留空格；保持 `0 Warning` |
| [13 声明出现在语句之后](https://jiangxiekeji.com/problem/p1-13.html) | C 标准/变量声明位置 | 本计划用 ArmClang 6.24；按工程标准配置 C99，或把声明移到块首，保持风格一致 |
| [14 `__INLINE` 等大量旧代码错误](https://jiangxiekeji.com/problem/p1-14.html) | 旧 CMSIS 与编译器不兼容 | 官方历史方案是切到 Arm Compiler 5.06；本计划锁定 ArmClang 6.24，必须使用课程要求的兼容 CMSIS/SPL V3.5.0 配置并修正兼容问题，不能静默切换工具链蒙混通过 |

## 8. 学习记录与证据清单

### 8.1 必交证据

- [ ] 开始前与结束后的 `git status --short`，并明确哪些差异是开始前就存在的。
- [ ] 今日工程绝对路径、基线时间、关键源码链接。
- [ ] P1～P4 笔记和 PPT 1～17 覆盖勾选。
- [ ] 芯片命名拆解、最小系统图、启动链、文件职责表。
- [ ] SWD 接线表与清晰接线照片。
- [ ] Keil **完整**构建日志，明确 `0 Error / 0 Warning`。
- [ ] `.map`/Build Summary 中的 Flash、RAM 使用量。
- [ ] 下载/调试证明；只凭编译日志不能证明硬件运行。
- [ ] 两项故障注入的“预测→现象→根因→恢复→复测”。
- [ ] 口试逐题原始回答，不得事后用标准答案覆盖。

证据状态统一写成：`VERIFIED`（Codex 可从文件/日志确认）、`USER_REPORTED`（只有你的陈述）、`NOT_PROVEN`（缺证据）、`BLOCKED`（工具或硬件客观阻塞）。

## 9. 交给 Codex 的只读验收流程

1. 提供今日工程路径、日志路径、证据路径、基线时间及开始/结束 Git 状态。
2. 明确要求 Codex **先审查、不得修改**；若发现问题先写报告，修复另开阶段。
3. Codex 阅读 `D:\STM32_Project\README.md`、`D:\STM32_Project\.github\AI_REVIEW.md`（若存在）、本计划、今日日志和上一次报告。
4. Codex 检查真实源码、`.uvprojx`、启动文件、宏、Include Paths、SPL 版本、ArmClang 版本、引脚表与日志一致性。
5. Codex 使用 `D:\keil\UV4\UV4.exe` 对今日工程做一次新构建，核对日志和 `.map`；不得用旧构建产物代替。
6. Codex 检查 Git 差异，区分既有改动与今日成果；不得触碰其他 OLED/EXTI 等未提交工程。
7. Codex 对硬件照片/下载记录只按证据强度标记，不能编造现象。
8. Codex 从下列类别随机出 **5～8 题，一次只出一题**，答完才评价并出下一题：
   - 2～3 题：ARM/STM32、系统结构、启动链、存储区或工具链原理；
   - 1～2 题：给定 BOOT/SWD/最小系统接线，预测启动或连接结果；
   - 1～2 题：从编译/链接/下载错误定位配置故障；
   - 1 题：闭卷从空目录重建最小 SPL 工程并解释启动链（今日特定迁移题）。
9. 如果答题时要求 Codex 给实现，该题标“有辅助”并更换参数补考；最终输出证据、缺项、分项得分、结论、重复问题和下一步。

## 10. 评分、硬门槛与结论

| 维度 | 分值 | 今日评分点 |
|---|---:|---|
| 原理与口述 | 20 | 芯片/内核关系、最小系统、启动链、存储与工具链 |
| 独立编码与迁移 | 30 | 闭卷建立工程、正确选择 V3.5.0/MD 启动文件/宏、独立配置 |
| 构建、下载与实物证据 | 25 | 新构建、0/0、map、SWD 接线、下载和运行证据 |
| 调试与边界处理 | 15 | 两项故障注入、第一错误定位、容量和工具链边界 |
| 日志与仓库卫生 | 10 | 链接准确、基线清楚、未污染其他工程、证据可追溯 |

硬门槛（任一未过，最高只能 `CONDITIONAL`）：

- 目标工程新构建 `0 Error / 0 Warning`。
- 有真实下载并执行到 `main()` 的证据；没有实物证据必须标 `NOT_PROVEN`。
- 能闭卷解释复位→`Reset_Handler`→`SystemInit()`→C 运行时→`main()`。
- 完成从空目录建立工程，不是复制 `2-1` 后改名。
- 工程确认只使用 CMSIS + SPL V3.5.0、ArmClang 6.24，无 HAL/LL/CubeMX/V3.6.x。

结论：`PASS`＝≥80 且硬门槛全过；`CONDITIONAL`＝70～79 或只缺一项可补证据（48 小时内补验）；`FAIL`＝<70、核心原理错误或迁移失败；`BLOCKED`＝因工具/硬件无法验证，不算学习失败但必须另约补验。

## 11. 可复制的 Codex 验收提示词

```text
请对我的 Day01 STM32F103 学习成果做严格验收。现在是验收阶段，只读检查，禁止修改任何源码、工程、日志和配置。
按北京时间（UTC+8）的 00:00～23:59 确定本次审查日期与变更范围。

固定标准：STM32F103C8T6；CMSIS + STM32F10x SPL V3.5.0；Keil MDK 5.43；ArmClang 6.24；禁止 HAL、LL、CubeMX 和 V3.6.x。
今日计划：D:\STM32_Project\00_Study_Log\14_Day_STM32_Bootcamp\Day01_Foundation_Project.md
工程路径：<填写绝对路径>
学习日志：<填写绝对路径>
证据目录/附件：<填写路径>
今日基线时间或提交：<填写>
开始/结束 git status：<填写或给路径>

请先阅读 D:\STM32_Project\README.md、D:\STM32_Project\.github\AI_REVIEW.md（若存在）、今日计划、日志和上次报告；核对真实源码、uvprojx、startup_stm32f10x_md.s、STM32F10X_MD、USE_STDPERIPH_DRIVER、Include Paths、编译器、SPL 版本和 Git 差异。使用 D:\keil\UV4\UV4.exe 对今日工程进行一次新构建，核对完整日志、0 Error / 0 Warning 和 map 中的 Flash/RAM。不要把编译成功等同于硬件成功；所有证据标记 VERIFIED、USER_REPORTED、NOT_PROVEN 或 BLOCKED，问题必须给绝对文件路径和行号。

静态审查后，从 Day01 的原理、接线预测、故障定位和迁移四类中随机出 5～8 题，一次只问一题，我回答后再评价并继续。迁移题必须让我从空目录重建最小 SPL 工程并解释复位到 main 的启动链；我若索要实现，则标“有辅助”并换参数补考。

最后按 20/30/25/15/10 共 100 分评分，检查全部硬门槛，给 PASS、CONDITIONAL、FAIL 或 BLOCKED，并列出证据、缺项、重复问题、48 小时补考项和下一步。经我明确确认后，才可把报告写入 D:\STM32_Project\AI_Review\YYYY-MM-DD_Day01_Foundation_Project.md；本次提示本身不授权写文件。
```

## 12. 当日复盘与补考

```text
日期（北京时间）：
实际用时：
工程路径：
今天我能不看资料讲清的三件事：
1.
2.
3.

最薄弱知识点：
第一次构建/下载错误：
我如何定位：
硬件实际现象：
故障注入得到的规律：
Flash/RAM 使用：
Codex 得分与结论：
VERIFIED / USER_REPORTED / NOT_PROVEN / BLOCKED 项：
明天开始前要复习的 3 项：
```

补考规则：48 小时内只补未通过维度，但必须使用新建目录、不同工程名并重新构建；Codex 随机改变一个条件（例如工具链配置缺项、启动文件候选或 SWD 故障）。原证据不能直接重复提交。核心启动链仍讲不清或闭卷工程仍失败，Day01 保持 `FAIL`，先补基础再进入 Day02。
