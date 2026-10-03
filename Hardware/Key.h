#ifndef KEY_H
#define KEY_H

#include "stm32f10x.h"

void Key_Init(void);

/* 返回0：没有按键 */
/* 返回1：模式键 */
/* 返回2：表情键 */
uint8_t Key_GetNum(void);

#endif

