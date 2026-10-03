#include "stm32f10x.h"
#include "OLED.h"
#include "Display.h"
#include "OLED_Data.h"
#include "Pet_Data.h"

/* 显示一个16×16汉字 */
void Display_ShowChinese(uint8_t Page, uint8_t Column, uint8_t Number)
{
    uint8_t Part;
    uint8_t i;

    for (Part = 0; Part < 2; Part++)
    {
        OLED_SetCursor(Page + Part, Column);

        for (i = 0; i < 16; i++)
        {
            OLED_WriteData(OLED_Chinese[Number][Part * 16 + i]);
        }
    }
}

/* 在“模式”后面显示数字1或2 */
void Display_ShowNumber(uint8_t Number)
{
    uint8_t Part;
    uint8_t i;

    for (Part = 0; Part < 2; Part++)
    {
        OLED_SetCursor(Part, 32);

        for (i = 0; i < 8; i++)
        {
            OLED_WriteData(OLED_Number[Number - 1][Part * 8 + i]);
        }
    }
}

/* 显示96×48猫猫图片 */
void Display_ShowPet(uint8_t State)
{
    uint8_t Page;
    uint8_t Column;

    for (Page = 0; Page < 6; Page++)
    {
        /* 第2页开始，对应屏幕第16行 */
        OLED_SetCursor(Page + 2, 16);

        for (Column = 0; Column < 96; Column++)
        {
            OLED_WriteData(Pet_Images[State - 1][Page * 96 + Column]);
        }
    }
}

/* 显示模式、状态文字和猫猫图片 */
void Display_Show(uint8_t Mode, uint8_t State)
{
    OLED_Clear();

    /* 左上角：“模式1”或“模式2” */
    Display_ShowChinese(0, 0, 0);
    Display_ShowChinese(0, 16, 1);
    Display_ShowNumber(Mode);

    /* 右上角：状态文字 */
    if (State == PET_HAPPY)
    {
        Display_ShowChinese(0, 96, 2);
        Display_ShowChinese(0, 112, 3);
    }
    else if (State == PET_SAD)
    {
        Display_ShowChinese(0, 96, 4);
        Display_ShowChinese(0, 112, 5);
    }
    else if (State == PET_SLEEP)
    {
        Display_ShowChinese(0, 96, 6);
        Display_ShowChinese(0, 112, 7);
    }

    /* 下方：对应的猫猫表情 */
    Display_ShowPet(State);
}

