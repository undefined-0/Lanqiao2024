#include "led.h"

// LED���˸�������ΪPC8-PC15
// ����������ΪPD2

void LED_Init(void)
{
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_All,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET); // �ߵ�ƽ����
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET); // �͵�ƽ����
}

void LED_Display(uint8_t i) // ����ת��Ϊ8λ�����ƺ� 1Ϊ��0Ϊ��
{
	LED_Init();
	HAL_GPIO_WritePin(GPIOC,i<<8,GPIO_PIN_RESET); // LEDΪ������  �͵�ƽʱ����
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET);
}
