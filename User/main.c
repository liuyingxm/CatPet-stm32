#include "stm32f10x.h"
#include "OLED.h"
#include "Display.h"
#include "Key.h"
#include "USART1.h"
#include <string.h>

uint8_t Mode = 1;
uint8_t State = PET_HAPPY;
uint8_t KeyNum;
uint8_t CommandValid;

int main(void)
{
    Key_Init();
    OLED_Init();
    USART1_Init();

    /* 上电为模式1，暂时不接收串口指令 */
    USART1_SetReceiveEnable(0);

    Display_Show(Mode, State);

    while (1)
    {
        KeyNum = Key_GetNum();

        /* PA6：切换模式 */
        if (KeyNum == 1)
        {
            if (Mode == 1)
            {
                Mode = 2;
                USART1_SetReceiveEnable(1);
            }
            else
            {
                Mode = 1;
                USART1_SetReceiveEnable(0);
            }

            Display_Show(Mode, State);
        }

        /* PA2：模式1下切换表情 */
        else if (KeyNum == 2 && Mode == 1)
        {
            State++;

            if (State > PET_SLEEP)
            {
                State = PET_HAPPY;
            }

            Display_Show(Mode, State);
        }

        /* 模式2下处理完整的串口指令 */
        if (Mode == 2 && USART1_GetRxFlag() == 1)
        {
            CommandValid = 1;

            if (strcmp(USART1_RxPacket, "HAPPY") == 0)
            {
                State = PET_HAPPY;
            }
            else if (strcmp(USART1_RxPacket, "SAD") == 0)
            {
                State = PET_SAD;
            }
            else if (strcmp(USART1_RxPacket, "SLEEP") == 0)
            {
                State = PET_SLEEP;
            }
            else
            {
                CommandValid = 0;
            }

            USART1_ClearRxFlag();

            if (CommandValid == 1)
            {
                Display_Show(Mode, State);
                USART1_SendString("OK\r\n");
            }
            else
            {
                USART1_SendString("UNKNOWN\r\n");
            }
        }
    }
}

