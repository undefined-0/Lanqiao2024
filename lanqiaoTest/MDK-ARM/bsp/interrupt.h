#ifndef __INTERRUPT_H__
#define __INTERRUPT_H__

#include "main.h"
#include "stdbool.h"

struct status // 名为status的结构体
{
	uint8_t step; // 进行到哪一步了
	bool key_status; // 识别到按键被按下时记录一个0
	bool key_flag; // 如果确认按键被按下，记录一个1
};

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim); // 中断回调函数

#endif /*__INTERRUPT_H__*/
