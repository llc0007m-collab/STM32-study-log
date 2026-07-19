# Day 03｜EXTI/NVIC 外部中断与 TIM 时基

> 建议用时：7.5 小时（6～8 小时）  
> 今日范围：视频 P11～P13；PPT 第 41～62 页；例程 5-1、5-2、6-1  
> 今日出口：闭卷完成一个可靠的 PB14 外部计次和一个 TIM2 周期中断；能迁移 EXTI 到不同引脚/中断向量，并用清晰的优先级理由处理多个中断。

## 1. 环境硬约束

- STM32F103C8T6；Keil MDK 5.43；ArmClang 6.24；CMSIS + **SPL V3.5.0**。
- V3.5.0：`D:\STM32_Library\固件库\固件库\STM32F10x_StdPeriph_Lib_V3.5.0\STM32F10x_StdPeriph_Lib_V3.5.0`。
- 使用 `STM32F10X_MD`、`USE_STDPERIPH_DRIVER`、`startup_stm32f10x_md.s`；禁止 V3.6.x/HAL/LL/CubeMX。
- 中断服务程序（ISR）只做必要采样、计数或置 `volatile` 标志；禁止 `Delay_ms`、OLED 刷屏和长循环。
- 课程附件只读；先保存 Git 基线，不覆盖现有 PB14/OLED/EXTI 未提交工程。

## 2. 精确课程映射

### 2.1 视频

| 顺序 | 视频分集原题 | 时长 | 必须形成的输出 |
|---|---|---:|---|
| P11 | `[5-1] EXTI外部中断` | 41:58 | 画 GPIO→AFIO→EXTI→NVIC→CPU 数据流与初始化顺序 |
| P12 | `[5-2] 对射式红外传感器计次&旋转编码器计次` | 49:31 | 对比单线计次与两相方向判断，写出抖动风险 |
| P13 | `[6-1] TIM定时中断` | 49:37 | 画时基单元，能从目标周期反算 PSC/ARR |

原速合计 2:21:06；中断分组、EXTI 线映射、定时公式必须暂停闭卷重画。

### 2.2 PPT、源码和手册

- PPT：`D:\STM32_Library\课件PPT.zip`（密码 `32`）内 `课件PPT\STM32入门教程.pptx` 第 41～62 页：中断/嵌套、NVIC 结构与分组、EXTI/AFIO、编码器、TIM 类型/结构、时基、预分频/计数/预装、RCC 时钟树。
- 例程根目录：`D:\STM32_Library\程序源码\程序源码\STM32Project-有注释版`：
  - `5-1 对射式红外传感器计次\Hardware\CountSensor.c`；
  - `5-2 旋转编码器计次\Hardware\Encoder.c`；
  - `6-1 定时器定时中断\System\Timer.c`。
- 官方勘误：`D:\STM32_Library\程序源码\程序源码\程序修改说明.pdf` 第 2～3 页（修改 1、2：两项 EXTI 计次进入 ISR 后再次读实际引脚电平）。
- 参考手册：`D:\STM32_Library\参考文档\参考文档\STM32F10xxx参考手册（中文）.pdf` 的 AFIO、EXTI、RCC、通用/高级定时器章节；`STM32F10xxx Cortex-M3编程手册.pdf` 的异常/NVIC；SPL 手册与 V3.5.0 的 `misc.c/.h`、`stm32f10x_exti.c/.h`、`stm32f10x_tim.c/.h`。

## 3. 7.5 小时时间表

| 时间 | 内容 | 当场证据 |
|---:|---|---|
| 00:00–00:20 | Git 基线；闭卷复习 GPIO 输入、有效电平、启动向量 | 基线和复习答卷 |
| 00:20–02:45 | P11～P13 + PPT 41～62 | 三张数据流图、22 页覆盖表 |
| 02:45–03:30 | 精读 5-1/5-2/6-1、勘误 1/2 和 SPL 状态位实现 | 视频版/最终版差异、API 表 |
| 03:30–04:35 | 闭卷完成 PB14 EXTI 计次，主循环显示/处理 | 自写驱动、0/0 构建 |
| 04:35–05:30 | 闭卷完成 TIM2 10 ms 节拍与 1 s 非阻塞任务 | 计算稿、测量结果 |
| 05:30–06:25 | 编码器双 EXTI、优先级设计、实物 50 次测试 | 正/反计数、误计数统计 |
| 06:25–06:55 | EXTI 换线迁移 + 三项故障注入 | 迁移差异和恢复证据 |
| 06:55–07:30 | Codex 只读审查、口试、复盘 | 评分与补考表 |

## 4. 完整知识清单

### 4.1 Cortex-M3/NVIC 中断模型

- [ ] 中断触发时 CPU 保存必要现场、从向量表取 ISR 地址、执行后异常返回；与普通函数调用不同。
- [ ] 区分中断源、外设状态/挂起位、外设中断使能、NVIC 使能、NVIC pending/active 和全局屏蔽；任何一级缺失都会“有状态但不进 ISR”。
- [ ] F103 的实现使用 4 个有效优先级位，数值越小优先级越高；`NVIC_PriorityGroup_0...4` 在抢占/响应位之间分配。
- [ ] 抢占优先级决定能否嵌套；抢占相同时响应优先级决定排队；两者都相同则按硬件中断号裁决。
- [ ] 优先级分组在系统中统一配置一次；各模块反复改分组会让已配置优先级语义变化。
- [ ] ISR 名称必须与中容量启动文件向量完全一致，例如 `EXTI0_IRQHandler`、`EXTI15_10_IRQHandler`、`TIM2_IRQHandler`。
- [ ] 主循环和 ISR 共享且会异步改变的对象使用 `volatile`；理解 `volatile` 不保证复合操作原子性、不解决竞态或内存一致性协议。
- [ ] 16/32 位单次访问是否原子不能替代业务一致性；读取多字段快照或“读后清零”应设计短临界区/单生产者协议。

### 4.2 AFIO/EXTI

- [ ] 所有 GPIO 可映射到 EXTI0～15，但同一编号一次只能选择一个端口：如 PA0/PB0/PC0 不能同时映射 EXTI0。
- [ ] 初始化链：GPIO 和 AFIO 时钟→GPIO 输入模式→`GPIO_EXTILineConfig` 选 Port/PinSource→EXTI 线/模式/触发→清 pending→NVIC 分组/通道/优先级→使能。
- [ ] 区分 `EXTI_Mode_Interrupt` 与 `EXTI_Mode_Event`，上升/下降/双边沿和软件触发。
- [ ] EXTI0～4 各有独立 IRQ；EXTI5～9 共用 `EXTI9_5_IRQn`；EXTI10～15 共用 `EXTI15_10_IRQn`。共享 ISR 必须逐线判断和清除。
- [ ] ISR 固定结构：判断相关 pending→确认实际输入/处理最小事件→清自己的 pending；不能无条件清除其他线。
- [ ] 机械/模块抖动可能同时产生多次上升和下降；选择边沿不等于消抖。再次读脚可滤掉部分假沿，但不是完整时间滤波。
- [ ] 正交编码器两相相差 90°；某相边沿到来时读取另一相即可判向；必须说明分辨率、边沿策略与计数正负号约定。

### 4.3 TIM 时基与时钟树

- [ ] 区分高级 TIM1、通用 TIM2/3/4（C8T6 可用资源）及基本定时器概念；不要因总系列手册列出外设就假定 C8T6 都具备。
- [ ] 时基：输入时钟 `CK_PSC`→预分频 PSC→计数时钟 `CK_CNT`→CNT→与 ARR 溢出/更新；16 位 PSC/CNT/ARR 的取值边界。
- [ ] APB1 通常 36 MHz，但当 APB1 预分频不为 1 时，TIM2/3/4 定时器时钟为 `2×PCLK1=72 MHz`；必须从实际 RCC 配置确认，不能直接把 PCLK1 当 TIM 时钟。
- [ ] `CK_CNT = CK_PSC / (PSC + 1)`。
- [ ] 向上计数更新频率 `f_update = CK_PSC / ((PSC + 1)(ARR + 1))`；周期 `T_update = (PSC + 1)(ARR + 1) / CK_PSC`。
- [ ] 配置“7200 分频、10000 次计数”应写 PSC=`7200-1`、ARR=`10000-1`；在 72 MHz 下为 1 s。
- [ ] 最大约 59.65 s 来自 `65536×65536/72 MHz`；实际还受时钟与模式影响。
- [ ] `TIM_ClockDivision` 配置数字滤波采样时钟分频，不是时基 PSC；`RepetitionCounter` 对通用 TIM2 不起教程所述用途。
- [ ] 预装寄存器让 ARR/PSC 在更新事件时生效，避免中周期突变；会区分立即装载/更新装载。
- [ ] `TIM_TimeBaseInit`/更新事件可能置 UIF；开启中断前清更新标志，避免一使能立刻进一次 ISR。
- [ ] 主循环用 ISR 增加 tick/置标志，实现非阻塞任务；不会在 TIM ISR 里 `OLED_Show...` 或延时。

### 4.4 状态位 API

- [ ] `GetFlagStatus` 直接看状态位；`ClearFlag` 清状态位。
- [ ] `GetITStatus` 同时要求状态位为 1 且对应中断已使能；`ClearITPendingBit` 清中断挂起/状态位（具体外设实现与参数范围要查源码）。
- [ ] 状态位为 1 不代表 ISR 一定执行：还需要外设中断使能、NVIC、优先级/屏蔽和正确向量。
- [ ] 清标志写法因外设不同，必须调用正确 SPL API 或依参考手册；不可把“写 0 清/写 1 清”跨外设套用。

## 5. 引脚/中断/时钟表

| 功能 | 课程参考 | 输入模式/映射 | IRQ/优先级 | 实际连接或参数 | 验证 |
|---|---|---|---|---|---|
| 对射传感器 DO | PB14 | 上拉输入，PB14→EXTI14，下降沿 | EXTI15_10；组2；1/1 | ______ | 50 次遮挡统计 |
| 编码器 A | PB0 | 上拉，PB0→EXTI0，下降沿 | EXTI0；1/1 | ______ | 正反方向 |
| 编码器 B | PB1 | 上拉，PB1→EXTI1，下降沿 | EXTI1；1/2 | ______ | 正反方向 |
| OLED SCL/SDA | PB8/PB9 | 开漏软件 I2C | 无 | ______ | 计数显示 |
| TIM2 内部时钟 | 无引脚 | APB1 定时器时钟 | TIM2；自行设计 | `CK_PSC=___, PSC=___, ARR=___` | 周期实测 |
| 迁移后的 EXTI | ______ | PortSource/PinSource/Line：______ | IRQ：______；抢占/响应：______ | ______ | 原线不再响应 |

优先级数字写“抢占/响应”，同时写设计理由；不要只抄课程的 1/1、1/2、2/1。

## 6. 闭卷编码、实物实验与迁移

### 6.1 必做实验

1. 独立写 `CountSensor`：PB14/EXTI14，ISR 只确认低电平、增加 `volatile` 计数或置标志、清 Line14；主循环 OLED 显示。
2. 每次遮挡/移开各算一次什么事件必须先定义。连续 50 次操作，记录期望/实际/多计/漏计；故意快速操作再比较。
3. 独立写 TIM2：推导并实现 10 ms 更新中断；ISR 只累加 tick，每 100 tick 在主循环完成一次可观察动作。示波器/逻辑分析仪测周期，或记录严格计时证据。
4. 独立写双 EXTI 编码器：PB0/PB1，明确边沿和判向真值表；正转/反转各 20 格，记录净计数及误差。
5. 全部工程新 Build `0 Error / 0 Warning`；保存 `.map`、下载和实物证据。

### 6.2 今日特定迁移题

Codex 从空闲且非 SWD 的引脚中指定一个**不同编号的 EXTI 线**，把 PB14 计次迁移过去；学习者必须独立修改 GPIO 时钟/端口/Pin、PortSource、PinSource、EXTI_Line、IRQChannel、ISR 名称/共享线判断和引脚读取，并重新设计与 TIM2 的优先级。先提交迁移表，后编码；原 PB14 不应再触发。

### 6.3 故障注入（至少三项）

- 故意写错 `GPIO_PortSource` 或 `GPIO_PinSource`，解释“IDR 会变但不进 ISR”。
- 不清 EXTI/TIM pending，观察重复进入或异常行为后恢复。
- ISR 名称拼错或 IRQ 选错，使用向量表/调试器定位。
- 将两个中断设为不同抢占/响应组合，用纸面时序先判断能否嵌套。
- 在 TIM 初始化后不清 UIF，观察首个中断时机；恢复后复测。

## 7. 官方勘误与 FAQ

- 勘误 1/2：`5-1` 和 `5-2` 最终版在进入 ISR 后再次读取触发脚，只有电平仍符合时才计数/判向，以缓解模块假沿。必须实现并说明局限：这不是通用消抖；若信号质量差，应考虑输入滤波、定时确认或硬件整形。
- [FAQ 8：两项 EXTI 计次数值抖动](https://jiangxiekeji.com/problem/p1-8.html)：模块抖动可让上/下降沿都成串出现，因此单改触发边沿可能无效；按最终源码二次读脚并做 50 次统计。
- [FAQ 18：四个状态/中断函数区别](https://jiangxiekeji.com/problem/p1-18.html)：能从 `SR/DIER`（TIM）或 `PR/IMR`（EXTI）解释 `GetFlagStatus/ClearFlag/GetITStatus/ClearITPendingBit`，并指出参数范围可能不同。

## 8. 证据清单

- [ ] Git 开始/结束状态、基线时间和既有未提交差异。
- [ ] P11～P13 笔记、PPT 41～62 覆盖、三张数据流/时序图。
- [ ] 例程与最终勘误差异；EXTI/NVIC/TIM API→寄存器对照。
- [ ] 实际引脚/EXTI 线/IRQ/优先级/时钟参数表。
- [ ] 公式手算、10 ms 测量、50 次传感器与正反各 20 格编码器统计。
- [ ] 完整新构建 0/0、map、下载、OLED/波形/照片证据。
- [ ] 三项故障注入和不同 EXTI 线迁移的前后证据。
- [ ] 口试原始回答与 Codex 报告。无硬件证明写 `NOT_PROVEN`。

状态：`VERIFIED`、`USER_REPORTED`、`NOT_PROVEN`、`BLOCKED`。

## 9. Codex 只读验收与口试

Codex 先读仓库 README、`.github\AI_REVIEW.md`（若有）、本计划、今日日志/证据和上次报告；只读检查源码、uvprojx、V3.5.0/ArmClang、EXTI 映射、共享 IRQ、NVIC 分组、`volatile`、ISR 阻塞、TIM 时钟来源/PSC/ARR、初始化清标志、引脚与日志。用 `D:\keil\UV4\UV4.exe` 新构建，核对 0/0 和 map；不修改。

随后随机 5～8 题、一次一题：2～3 题中断/NVIC/EXTI/TIM 原理与公式；1～2 题给输入波形和优先级预测执行；1～2 题从不进 ISR/计数异常/周期错误定位；1 题把 EXTI 迁移到不同线并重新设计优先级。索要实现则标“有辅助”并换线补考。

## 10. 100 分评分和硬门槛

| 维度 | 分值 | 今日重点 |
|---|---:|---|
| 原理与口述 | 20 | NVIC 分组、EXTI 数据流、状态位、TIM 时钟/公式 |
| 独立编码与迁移 | 30 | PB14、双相编码器、TIM tick、不同 EXTI 线迁移 |
| 构建/下载/实物 | 25 | 0/0、map、统计、周期和实物证据 |
| 调试与边界 | 15 | 抖动、共享 IRQ、竞态、三项故障注入 |
| 日志/仓库卫生 | 10 | 证据链接、基线、未碰其他工程 |

硬门槛：0 Error/0 Warning；真实 EXTI+TIM 运行证据；闭卷推导一个定时参数并解释中断全链；迁移到不同编号 EXTI 线成功；ISR 无延时/OLED/长循环且共享状态规则明确；仅 V3.5.0。任一缺失最高 `CONDITIONAL`。

`PASS`：≥80 且硬门槛全过；`CONDITIONAL`：70～79 或只缺一项可补证据（48 小时）；`FAIL`：<70、不能解释映射/优先级/公式或迁移失败；`BLOCKED`：工具/硬件阻塞，必须补验。

## 11. 可复制 Codex 提示词

```text
请只读验收我的 STM32 Day03 EXTI/NVIC/TIM 时基成果，禁止修改任何文件。
标准：STM32F103C8T6，CMSIS+SPL V3.5.0，Keil MDK 5.43，ArmClang 6.24；禁用 V3.6.x/HAL/LL/CubeMX。
计划：D:\STM32_Project\00_Study_Log\14_Day_STM32_Bootcamp\Day03_EXTI_TIM_Base.md
工程：<绝对路径>；日志：<绝对路径>；证据：<路径/附件>
基线时间/提交：<填写>；开始/结束 git status：<填写>

先读 README.md、.github\AI_REVIEW.md（若存在）、计划和上次报告。核对真实源码/uvprojx、工具链、GPIO→AFIO→EXTI→NVIC、Line/IRQ/ISR、共享向量、优先级分组、volatile/竞态、ISR 阻塞、勘误二次读脚、TIM2 时钟来源与 PSC/ARR/清 UIF。使用 D:\keil\UV4\UV4.exe 新构建并检查 0 Error/0 Warning、map。硬件只按证据标 VERIFIED/USER_REPORTED/NOT_PROVEN/BLOCKED；问题给绝对路径和行号。

审查后从原理/公式、波形与优先级预测、故障定位、迁移中随机问 5～8 题，一次一题。迁移题请指定一个空闲、非 SWD、且编号不同于 14 的 EXTI 引脚，让我独立改 Port/Pin/Source/Line/IRQ/ISR 并设计和 TIM2 的优先级；索要实现就换题补考。

按 20/30/25/15/10 评分，检查硬门槛，给 PASS/CONDITIONAL/FAIL/BLOCKED、证据、缺项、重复问题和 48 小时补考。未经我之后明确授权，不写 AI_Review 报告文件。
```

## 12. 复盘/补考

```text
日期/用时：
我画出的中断全链：
EXTI 线与 IRQ 最易混点：
TIM 时钟/PSC/ARR 手算：
传感器 50 次：期望___ 实际___ 多计___ 漏计___
编码器正/反统计：
ISR 中共享变量策略：
三个故障及定位：
迁移结果：
Codex 得分/结论：
NOT_PROVEN/BLOCKED：
Day04 前复习项：
```

补考必须更换 EXTI 编号和定时周期，重新手算、新构建、新下载并提交新统计；不得复用旧照片/构建日志。核心中断链或 TIM 公式仍不清楚则保持 `FAIL`。
