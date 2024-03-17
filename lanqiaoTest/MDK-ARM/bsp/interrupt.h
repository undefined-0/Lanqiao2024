#ifndef __INTERRUPT_H__
#define __INTERRUPT_H__

#include "main.h"
#include "stdbool.h"

struct status // ��Ϊstatus�Ľṹ��
{
	uint8_t step; // ���е���һ����
	bool key_status; // ʶ�𵽰���������ʱ��¼һ��0
	bool key_short_flag; // �����̰���־λ��1Ϊ���£�0Ϊδ���£�
	bool key_long_flag; // ����������־λ��1Ϊ�ѳ�����
	uint16_t key_time; // �������µ�ʱ��
};

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim); // �жϻص�����

#endif /*__INTERRUPT_H__*/
