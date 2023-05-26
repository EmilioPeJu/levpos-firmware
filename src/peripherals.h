#ifndef _PERIPHERALS_H
#define _PERIPHERALS_H

#include "main.h"

extern UART_HandleTypeDef huart2;
extern TIM_HandleTypeDef htim1;

#define HOST_HUART huart2
#define WAVE_TIMER htim1

#endif
