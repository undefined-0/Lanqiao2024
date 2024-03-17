#include "interrupt.h"

/* 
����ԭ��ͼ��
��������->����Ϊ�͵�ƽ0
����δ����->����Ϊ�ߵ�ƽ1
*/

struct status key[4] = {{0,0,0},{0,0,0},{0,0,0},{0,0,0}};
// �ĸ�����->�ĸ��ṹ�����
// ÿ���ṹ���������������������ֵΪ0

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) // �жϻص�����
{
	if(htim->Instance == TIM3) //�ж��Ƿ�������TIM3���ж�
	{
		// ��ȡ�ĸ�������״̬�����ŵ�ƽ��
		key[0].key_status = HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0); // PB0
		key[1].key_status = HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_1); // PB1
		key[2].key_status = HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_2); // PB2
		key[3].key_status = HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0); // PA0
		
		// ��ѯ
		for(uint8_t i=0; i<4; i++)
		{
			switch(key[i].step)
			{
				case 0: //�жϰ���
				{
					if(key[i].key_status == 0) // ͨ��if��ʼʱ�ļ�⣬���ְ������ܱ������ˣ���⵽����״̬��1��0����������ȷ�����Ŷ����ǰ�����ı����£�
						key[i].step = 1; // ����״̬���е���һ��
				}
				break;
				
				case 1: // ��������
				{
					if(key[i].key_status == 0) // 10ms���ټ�����ŵ�ƽ��������ǵ͵�ƽ����˵��������ı�������
					{	key[i].step = 2; // ����״̬���е��ڶ���
						key[i].key_flag = 1; // ������ı�������
					}
					else key[i].step = 0; // ������û�б����£�ֻ���ܵ����Ŷ������Դ�ʱ������״̬�˻ص���ʼ״̬����step��ֵΪ0����
				}
				break;
				
				case 2: // �ж�����
				{
					if(key[i].key_status == 1) // ����״̬��0��1��˵���������ɿ���
					{
						key[i].step = 0; // ������״̬�˻ص���ʼ״̬����step��ֵΪ0��
					}
				}
				break;
			}
		} // ��ѯ���������¶�ȡ�ĸ�����������״̬
	}
} // �жϰ������жϻص���������
