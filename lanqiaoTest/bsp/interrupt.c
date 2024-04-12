#include "interrupt.h"
#include "usart.h"

/* 
����ԭ��ͼ��
��������->����Ϊ�͵�ƽ0
����δ����->����Ϊ�ߵ�ƽ1
*/

struct status key[4] = {{0,0,0},{0,0,0},{0,0,0},{0,0,0}};
// �ĸ�����->�ĸ��ṹ�����
// ÿ���ṹ���������������������ֵΪ0

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) // �жϻص����������ж�ÿ10ms����һ�Σ�
{
	if(htim->Instance == TIM4) //�ж��Ƿ�������TIM4���ж�
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
					{
						key[i].step = 1; // ����״̬���е���һ��
						key[i].key_time = 0; // ���ְ������ܱ����º�ʱ������
					}
				}
				break;
				
				case 1: // ��������
				{
					if(key[i].key_status == 0) // 10ms���ټ�����ŵ�ƽ��������ǵ͵�ƽ����˵��������ı�������
					{	
						key[i].step = 2; // ����״̬���е��ڶ���
					}
					else key[i].step = 0; // ������û�б����£�ֻ���ܵ����Ŷ������Դ�ʱ������״̬�˻ص���ʼ״̬����step��ֵΪ0����
				}
				break;
				
				case 2: // �ж�����
				{
					if(key[i].key_status == 1) // ����״̬��0��1��˵���������ɿ���
					{
						key[i].step = 0; // ������״̬�˻ص���ʼ״̬����step��ֵΪ0��
						
						if (key[i].key_time < 70) // �����ֵ�ʱ��û����700ms��0.7s��
							key[i].key_short_flag = 1; // ���̰���־λ��Ϊ1
					}
					else // ����״̬û�䣬˵���������ڱ�����
					{
						key[i].key_time++; // ����û���ֵ�ʱ�䣨10ms����һ�Σ�
						if (key[i].key_time > 70) // �����ֵ�ʱ�䳬��700ms��0.7s��
							key[i].key_long_flag = 1; // ��������־λ��Ϊ1
					}
				}
				break;
			}
		} // ��ѯ���������¶�ȡ�ĸ�����������״̬
	}
} // �жϰ������жϻص���������

uint16_t ccrl_val_1,ccrl_val_2 = 0; // ע�⣬����Ҫʹ��uint16_t����uint8_t�������������
uint16_t frq_1,frq_2 = 0; // ע�⣬����Ҫʹ��uint16_t����uint8_t�������������
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim) // �жϻص�����
{
	if(htim->Instance == TIM2) //�ж��Ƿ�������TIM2���ж�
	{
		ccrl_val_1 = HAL_TIM_ReadCapturedValue(htim,TIM_CHANNEL_1); // �Ѷ�ʱ���ļ�ʱֵ�浽һ��������
		__HAL_TIM_SetCounter(htim,0); // ��ʱֵ����֮��Ϳ���������
		frq_1 = (80000000/80)/ccrl_val_1; // ����Ƶ�ʣ���80MHz���Է�Ƶϵ��80���ٳ��Ըոյõ��ļ�ʱֵ
		HAL_TIM_IC_Start(htim,TIM_CHANNEL_1);
	}
	
	if(htim->Instance == TIM3) //�ж��Ƿ�������TIM3���ж�
	{
		ccrl_val_2 = HAL_TIM_ReadCapturedValue(htim,TIM_CHANNEL_1); // �Ѷ�ʱ���ļ�ʱֵ�浽һ��������
		__HAL_TIM_SetCounter(htim,0); // ��ʱֵ����֮��Ϳ���������
		frq_2 = (80000000/80)/ccrl_val_2; // ����Ƶ�ʣ���80MHz���Է�Ƶϵ��80���ٳ��Ըոյõ��ļ�ʱֵ
		HAL_TIM_IC_Start(htim,TIM_CHANNEL_1);
	}
}

char rxdata[30]; // ���������Ѿ�����õ�����
uint8_t rxdat; // ÿ�ν���ʱ�����洢�ַ��ı���
uint8_t rx_pointer; // ��ָ�롱��ȷ������д���ĸ�λ�ã���Ϊ��һλһλ���յģ�
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) // ���ڽ��յĻص�����
{
	rxdata[rx_pointer++] = rxdat; // ÿ��ֻ�ܽ���һλ��ÿ����һ���жϾͻ����һ�ε��ȴ浽�����һλһλƴ�����͵õ��˴���õ�����
	HAL_UART_Receive_IT(&huart1,&rxdat,1); // λ���������1����Ϊÿ���ж�ֻ�ܽ���һ���ַ�
}

















