#ifndef OLED_H
#define OLED_H

#include "stm32f10x.h"

void OLED_Init(void);
void OLED_Clear(void);

void OLED_WriteCommand(uint8_t Command);
void OLED_WriteData(uint8_t Data);

/* Page：0~7；Column：0~127 */
void OLED_SetCursor(uint8_t Page, uint8_t Column);

#endif

