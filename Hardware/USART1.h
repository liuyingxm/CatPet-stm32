#ifndef USART1_H
#define USART1_H

#include "stm32f10x.h"

/* 保存收到的文字指令 */
extern char USART1_RxPacket[16];

void USART1_Init(void);

void USART1_SendByte(uint8_t Byte);
void USART1_SendString(const char *String);

/* 返回1表示收到完整指令，返回0表示还没有 */
uint8_t USART1_GetRxFlag(void);
void USART1_ClearRxFlag(void);

/* Enable为1允许接收指令，为0忽略指令 */
void USART1_SetReceiveEnable(uint8_t Enable);

#endif

