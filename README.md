# CatPet-stm32

基于 **STM32F103C8T6 + 128×64 OLED** 的猫猫表情小项目。支持两个按键和有线 USART 控制，显示“开心、委屈、睡觉”三种表情。

采用 Keil MDK 5、STM32 标准外设库、GPIO 轮询按键和软件 I²C，适合 STM32 初学者按模块学习。

## 功能

- 上电显示 **模式1 / 开心**。
- **模式1：按键控制。** 表情键每按下并松开一次，按“开心 → 委屈 → 睡觉 → 开心”切换。
- **模式2：串口控制。** 电脑通过 USB 转串口模块发送 `HAPPY`、`SAD`、`SLEEP`，选择对应表情。
- 模式键可以在模式1与模式2之间切换，保留当前表情。
- OLED 左上角显示模式，右上角显示状态，下方显示猫猫图片。
- 所选表情保持到下一次手动操作。

## 像素预览

以下预览由仓库里的字模和图片数组解码生成，展示画面布局，**不是实机照片**。

| 模式1：开心 | 模式1：委屈 | 模式2：睡觉 |
|---|---|---|
| <img src="docs/preview/mode1-happy.png" width="256" alt="模式1 开心"> | <img src="docs/preview/mode1-sad.png" width="256" alt="模式1 委屈"> | <img src="docs/preview/mode2-sleep.png" width="256" alt="模式2 睡觉"> |

屏幕顶部使用16行像素显示文字；三张猫猫图片统一为96×48，在 `(16, 16)` 位置显示。

## 硬件与接线

需要 STM32F103C8T6 开发板、四针 I²C OLED、两个独立按键、USB 转串口模块、ST-Link 和连接线。

| 模块接口 | STM32接口 | 说明 |
|---|---|---|
| OLED GND | GND | 电源地 |
| OLED VCC / VDD | 3.3V | 屏幕供电 |
| OLED SCL / SCK | PB8 | 软件 I²C 时钟 |
| OLED SDA | PB9 | 软件 I²C 数据 |
| 模式切换键 | PA6 与 GND之间 | 内部上拉，按下为低电平 |
| 表情切换键 | PA2 与 GND之间 | 内部上拉，按下为低电平 |
| USB转串口 TXD | PA10 / USART1_RX | 模块发送接芯片接收 |
| USB转串口 RXD | PA9 / USART1_TX | 模块接收接芯片发送 |
| USB转串口 GND | GND | 与开发板共地 |
| ST-Link SWDIO | PA13 / SWDIO | 下载、调试 |
| ST-Link SWCLK | PA14 / SWCLK | 下载、调试 |
| ST-Link GND | GND | 调试共地 |

USB 转串口使用 **3.3V 信号电平**。OLED 供电接真实的3.3V和GND；软件中只配置 PB8、PB9 作为通信引脚。四脚按键选用按下才导通的两个端子。

OLED 驱动按 **SSD1306兼容、128×64** 配置，默认7位地址为 `0x3C`，写地址为 `0x78`。如果模块地址为 `0x3D`，将 `Hardware/OLED.c` 中 `OLED_ADDRESS` 改为 `0x7A`。其他控制器型号需要核对相应初始化和寻址方式。

## 编译与下载

1. 安装 Keil MDK 5、Arm Compiler 5 和 STM32F1xx Device Family Pack。
2. 用 Keil 打开根目录的 **`CatPet-stm32.uvprojx`**。
3. 工程已设置 `STM32F103C8`、`USE_STDPERIPH_DRIVER,STM32F10X_MD` 和各模块的头文件路径。
4. 选择 `Project → Rebuild all target files` 重新编译。
5. 在 `Options for Target → Debug` 中选择 **ST-Link Debugger**。
6. 点击 `Settings`，在 Debug 页设置 **Port = SW**，确认识别到目标芯片。
7. 在 Flash Download 页确认 STM32F10x Med-density 编程算法，并勾选 **Reset and Run**。
8. 在 Utilities 页选择 **Use Target Driver for Flash Programming**，保存设置后按 **F8** 下载。

工程使用 ARM Compiler 5；开发时使用的版本为 **V5.06 update 5 (build 528)**，工程包引用 `Keil.STM32F1xx_DFP.2.2.0`。工具链未安装时，需先安装对应组件或选择兼容的本地工具链。

时钟代码按外部8MHz晶振配置72MHz系统时钟；`System/Delay.c` 也按72MHz计算延时。使用不同晶振或时钟配置的开发板时，需要同步调整。

2026-10-03，整理后的工程副本已使用 ARM Compiler V5.06 update 5 全量重新编译，结果为 **0 Error(s), 0 Warning(s)**。构建记录见 [docs/BUILD_VERIFICATION.md](docs/BUILD_VERIFICATION.md)。这项记录验证工程的编译与链接，不代表所有硬件连接和操作场景均已测试。

仓库仅保存 `.uvprojx` 工程配置。本机的 `.uvoptx`、`.uvguix.*` 和调试记录不参与上传，因此新电脑第一次下载时需要选择 ST-Link。

## 串口使用

串口助手选择 USB 转串口模块对应的 COM 口，设置 **9600波特率、8数据位、无校验、1停止位（8N1）**，采用文本 / ASCII 发送，并附加回车换行。

按 PA6 切换到模式2，再发送大写指令：

| 指令 | 状态 | 回复 |
|---|---|---|
| `HAPPY` | 开心 | `OK` |
| `SAD` | 委屈 | `OK` |
| `SLEEP` | 睡觉 | `OK` |
| 其他文本 | 保留原状态 | `UNKNOWN` |

每条指令以回车 `\r` 或换行 `\n` 结束，`\r\n` 也可以。收到 `OK` 后再发送下一条。接收数组为16字节，最多保存15个字符；这是面向单条短指令的课堂式接收方法。

模式1下接收到的字符被忽略；切换模式时清除尚未处理的接收内容。按键采用20ms消抖和等待松开的写法，松开之后执行对应操作。

## 工程结构

```text
CatPet-stm32/
├── CatPet-stm32.uvprojx    Keil工程
├── Start/                 Cortex-M3与STM32F103启动文件
├── Library/               STM32F10x标准外设库
├── System/                Delay.c/h
├── Hardware/
│   ├── OLED.c/h           软件I²C与OLED底层驱动
│   ├── OLED_Data.h        中文及数字字模
│   ├── Pet_Data.h         三张96×48表情图片
│   ├── Display.c/h        模式、状态文字与图片显示
│   ├── Key.c/h            PA6、PA2按键
│   └── USART1.c/h         串口初始化、中断接收、文字发送
├── User/
│   ├── main.c             模式判断与模块调用
│   └── stm32f10x_conf.h    标准库配置
├── docs/preview/           数组解码的像素预览
├── .gitignore             排除编译产物和本机设置
├── .gitattributes         文本与图片文件属性
└── THIRD_PARTY_NOTICES.md  第三方来源说明
```

`OLED_Data.h` 和 `Pet_Data.h` 保存固定数组，只由 `Display.c` 包含，不需要对应的 `.c` 文件。应用逻辑从 `User/main.c` 开始阅读，再查看 Key、USART1、Display 和 OLED 模块。

## 常见问题

- **No ULINK2/ME Device found：** Debug页选择ST-Link，Utilities页使用目标调试驱动下载。
- **屏幕没有内容：** 核对供电、PB8/PB9接线、OLED地址和型号；编译通过不代表接线已验证。
- **按键一直无响应：** 检查按键端子是否常通，并确认按下接地、松开恢复高电平。
- **串口没有回复：** 先切换模式2，检查COM口、9600波特率、TX/RX交叉连接、共地和命令结束符。
- **文件末尾换行警告：** 源文件最后一行之后保留换行并保存，再重新编译。

## 来源

标准外设库与CMSIS文件保留原始版权说明。猫猫像素图参考公开的 [STM32F103C8T6-desktop-pet](https://github.com/URNR0/STM32F103C8T6-desktop-pet)，并整理为当前显示布局；本项目采用固定表情和两种手动控制模式。

详细来源见 [THIRD_PARTY_NOTICES.md](THIRD_PARTY_NOTICES.md)，上传步骤见 [docs/GITHUB_UPLOAD.md](docs/GITHUB_UPLOAD.md)。
