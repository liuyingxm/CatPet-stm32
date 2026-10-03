# 第三方来源说明

## STM32标准外设库

`Library/` 中的文件来自 STMicroelectronics STM32F10x Standard Peripheral Library V3.5.0；`User/stm32f10x_conf.h` 来自其工程配置模板。
文件中的原始版权和使用说明予以保留。

## CMSIS和启动文件

`Start/core_cm3.c`、`Start/core_cm3.h` 来自 Arm Cortex-M3 CMSIS基础文件；其余芯片定义、系统时钟和启动文件来自 STM32F10x 标准库配套文件。
文件中的原始版权和使用说明予以保留。

## 猫猫像素图

`Hardware/Pet_Data.h` 的图像参考：

- 仓库：<https://github.com/URNR0/STM32F103C8T6-desktop-pet>
- 原始数据：`User/pet_frames.h`
- 使用表情：happy、sad、sleep
- 本工程处理：128×64图像整理为96×48，转换为按页、按列排列的数据数组。

原始素材及第三方文件的授权，以其来源声明为准。本仓库未对所有第三方内容另行指定统一许可证。

## 字模

`Hardware/OLED_Data.h` 中的16×16汉字和8×16数字字模，由电脑上的宋体字形转换为单色像素数组，用于当前项目的固定文字显示。
