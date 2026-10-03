#ifndef DISPLAY_H
#define DISPLAY_H

#include "stm32f10x.h"

#define PET_HAPPY 1
#define PET_SAD   2
#define PET_SLEEP 3

/* Mode：1按键模式，2串口模式 */
/* State：1开心，2委屈，3睡觉 */
void Display_Show(uint8_t Mode, uint8_t State);

#endif

