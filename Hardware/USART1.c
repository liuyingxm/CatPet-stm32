#include "stm32f10x.h"
#include "USART1.h"

char USART1_RxPacket[16];

/* 收完一条指令的标志 */
static volatile uint8_t RxFlag = 0;

/* 是否允许接收指令 */
static volatile uint8_t RxEnabled = 0;

/* 当前接收位置 */
static volatile uint8_t RxIndex = 0;

void USART1_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB2PeriphClockCmd(
        RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1,
        ENABLE
    );

    /* PA9：串口发送 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* PA10：串口接收 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* 串口参数：9600、8位数据、无校验、1位停止位 */
    USART_InitStructure.USART_BaudRate = 9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
    USART_InitStructure.USART_HardwareFlowControl =
        USART_HardwareFlowControl_None;

    USART_Init(USART1, &USART_InitStructure);

    /* 开启接收中断 */
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    RxFlag = 0;
    RxIndex = 0;
    RxEnabled = 0;
    USART1_RxPacket[0] = '\0';

    USART_Cmd(USART1, ENABLE);
}

/* 发送一个字节 */
void USART1_SendByte(uint8_t Byte)
{
    USART_SendData(USART1, Byte);

    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
    {
    }
}

/* 发送一串文字 */
void USART1_SendString(const char *String)
{
    uint16_t i;

    for (i = 0; String[i] != '\0'; i++)
    {
        USART1_SendByte(String[i]);
    }
}

/* 查看是否收到完整指令 */
uint8_t USART1_GetRxFlag(void)
{
    return RxFlag;
}

/* 指令处理完后，允许接收下一条 */
void USART1_ClearRxFlag(void)
{
    USART1_RxPacket[0] = '\0';
    RxFlag = 0;
}

/* 切换模式时调用，清除原来尚未处理的指令 */
void USART1_SetReceiveEnable(uint8_t Enable)
{
    RxEnabled = 0;

    RxFlag = 0;
    RxIndex = 0;
    USART1_RxPacket[0] = '\0';

    if (Enable != 0)
    {
        RxEnabled = 1;
    }
}

/* USART1接收中断 */
void USART1_IRQHandler(void)
{
    uint8_t RxData;

    if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
    {
        /* 读取数据后，RXNE标志自动清除 */
        RxData = (uint8_t)USART_ReceiveData(USART1);

        if (RxEnabled == 1 && RxFlag == 0)
        {
            /* 回车或换行表示一条指令结束 */
            if (RxData == '\r' || RxData == '\n')
            {
                if (RxIndex > 0)
                {
                    USART1_RxPacket[RxIndex] = '\0';
                    RxIndex = 0;
                    RxFlag = 1;
                }
            }
            else
            {
                /* 留一个位置保存字符串结束符 */
                if (RxIndex < 15)
                {
                    USART1_RxPacket[RxIndex] = (char)RxData;
                    RxIndex++;
                }
            }
        }
    }
}

