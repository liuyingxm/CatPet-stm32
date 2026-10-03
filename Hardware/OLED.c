#include "stm32f10x.h"
#include "OLED.h"
#include "Delay.h"

/* OLED写地址 */
#define OLED_ADDRESS 0x78

/* 控制SCL：PB8 */
void OLED_W_SCL(uint8_t BitValue)
{
    if (BitValue == 0)
    {
        GPIO_ResetBits(GPIOB, GPIO_Pin_8);
    }
    else
    {
        GPIO_SetBits(GPIOB, GPIO_Pin_8);
    }
}

/* 控制SDA：PB9 */
void OLED_W_SDA(uint8_t BitValue)
{
    if (BitValue == 0)
    {
        GPIO_ResetBits(GPIOB, GPIO_Pin_9);
    }
    else
    {
        GPIO_SetBits(GPIOB, GPIO_Pin_9);
    }
}

/* I2C起始信号 */
void OLED_I2C_Start(void)
{
    OLED_W_SDA(1);
    Delay_us(2);

    OLED_W_SCL(1);
    Delay_us(2);

    OLED_W_SDA(0);
    Delay_us(2);

    OLED_W_SCL(0);
    Delay_us(2);
}

/* I2C停止信号 */
void OLED_I2C_Stop(void)
{
    OLED_W_SCL(0);
    Delay_us(2);

    OLED_W_SDA(0);
    Delay_us(2);

    OLED_W_SCL(1);
    Delay_us(2);

    OLED_W_SDA(1);
    Delay_us(2);
}

/* I2C发送一个字节 */
void OLED_I2C_SendByte(uint8_t Byte)
{
    uint8_t i;

    for (i = 0; i < 8; i++)
    {
        /* 发送最高位 */
        if ((Byte & 0x80) != 0)
        {
            OLED_W_SDA(1);
        }
        else
        {
            OLED_W_SDA(0);
        }
        Delay_us(2);

        OLED_W_SCL(1);
        Delay_us(2);

        OLED_W_SCL(0);
        Delay_us(2);

        /* 将下一位移到最高位 */
        Byte = Byte << 1;
    }

    /* 释放SDA，第9个时钟留给OLED应答 */
    OLED_W_SDA(1);
    Delay_us(2);

    OLED_W_SCL(1);
    Delay_us(2);

    OLED_W_SCL(0);
    Delay_us(2);
}

/* 发送命令 */
void OLED_WriteCommand(uint8_t Command)
{
    OLED_I2C_Start();

    OLED_I2C_SendByte(OLED_ADDRESS);
    OLED_I2C_SendByte(0x00);
    OLED_I2C_SendByte(Command);

    OLED_I2C_Stop();
}

/* 发送显示数据 */
void OLED_WriteData(uint8_t Data)
{
    OLED_I2C_Start();

    OLED_I2C_SendByte(OLED_ADDRESS);
    OLED_I2C_SendByte(0x40);
    OLED_I2C_SendByte(Data);

    OLED_I2C_Stop();
}

/* 设置显示位置 */
void OLED_SetCursor(uint8_t Page, uint8_t Column)
{
    OLED_WriteCommand(0xB0 | Page);
    OLED_WriteCommand(0x10 | (Column >> 4));
    OLED_WriteCommand(Column & 0x0F);
}

/* 清屏 */
void OLED_Clear(void)
{
    uint8_t Page;
    uint8_t Column;

    for (Page = 0; Page < 8; Page++)
    {
        OLED_SetCursor(Page, 0);

        for (Column = 0; Column < 128; Column++)
        {
            OLED_WriteData(0x00);
        }
    }
}

/* OLED初始化 */
void OLED_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    OLED_W_SCL(1);
    OLED_W_SDA(1);

    Delay_ms(100);

    OLED_WriteCommand(0xAE);     /* 关闭显示 */

    OLED_WriteCommand(0xD5);     /* 显示时钟 */
    OLED_WriteCommand(0x80);

    OLED_WriteCommand(0xA8);     /* 显示64行 */
    OLED_WriteCommand(0x3F);

    OLED_WriteCommand(0xD3);     /* 显示偏移 */
    OLED_WriteCommand(0x00);

    OLED_WriteCommand(0x40);     /* 显示起始行 */

    OLED_WriteCommand(0xA1);     /* 列扫描方向 */
    OLED_WriteCommand(0xC8);     /* 行扫描方向 */

    OLED_WriteCommand(0xDA);     /* COM引脚配置 */
    OLED_WriteCommand(0x12);

    OLED_WriteCommand(0x81);     /* 对比度 */
    OLED_WriteCommand(0x7F);

    OLED_WriteCommand(0xD9);     /* 预充电周期 */
    OLED_WriteCommand(0xF1);

    OLED_WriteCommand(0xDB);     /* VCOMH电平 */
    OLED_WriteCommand(0x30);

    OLED_WriteCommand(0x20);     /* 页寻址模式 */
    OLED_WriteCommand(0x02);

    OLED_WriteCommand(0x2E);     /* 关闭滚动 */
    OLED_WriteCommand(0xA4);     /* 按存储器内容显示 */
    OLED_WriteCommand(0xA6);     /* 正常显示 */

    OLED_WriteCommand(0x8D);     /* 开启电荷泵 */
    OLED_WriteCommand(0x14);

    OLED_Clear();

    OLED_WriteCommand(0xAF);     /* 开启显示 */
}

