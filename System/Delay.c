#include "stm32f10x.h"
#include "Delay.h"

/* 微秒延时 */
void Delay_us(uint32_t xus)
{
    SysTick->LOAD = 72 * xus;             //设置计数值
    SysTick->VAL = 0x00;                  //清空当前计数
    SysTick->CTRL = 0x00000005;           //启动计数

    while (!(SysTick->CTRL & 0x00010000)); //等待计数结束

    SysTick->CTRL = 0x00000004;           //关闭计数
}

/* 毫秒延时 */
void Delay_ms(uint32_t xms)
{
    while (xms--)
    {
        Delay_us(1000);
    }
}

