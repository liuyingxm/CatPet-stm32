#include "stm32f10x.h"
#include "Key.h"
#include "Delay.h"

void Key_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

uint8_t Key_GetNum(void)
{
    uint8_t KeyNum = 0;

    /* PA6：模式切换键 */
    if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6) == 0)
    {
        Delay_ms(20);

        if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6) == 0)
        {
            /* 等待按键松开 */
            while (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6) == 0)
            {
            }

            Delay_ms(20);
            KeyNum = 1;
        }
    }
    /* PA2：表情切换键 */
    else if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_2) == 0)
    {
        Delay_ms(20);

        if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_2) == 0)
        {
            /* 等待按键松开 */
            while (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_2) == 0)
            {
            }

            Delay_ms(20);
            KeyNum = 2;
        }
    }

    return KeyNum;
}

