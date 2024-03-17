#ifndef __INTERRUPT_H__
#define __INTERRUPT_H__

#include "main.h"
#include "stdbool.h"

struct status // 名为status的结构体
{
	uint8_t step; // 进行到哪一步了
	bool key_status; // 识别到按键被按下时记录一个0
	bool key_short_flag; // 按键短按标志位（1为按下，0为未按下）
	bool key_long_flag; // 按键长按标志位（1为已长按）
	uint16_t key_time; // 按键按下的时间
};

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim); // 中断回调函数

#endif /*__INTERRUPT_H__*/
