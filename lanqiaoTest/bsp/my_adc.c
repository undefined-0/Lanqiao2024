#include "my_adc.h"

double getADC(ADC_HandleTypeDef *pin) // ��ȡADCֵ�ĺ�����������ADC������
{
	uint16_t adc_value; // �½�����������ADCֵ
	HAL_ADC_Start(pin); // ����ADC
	adc_value = HAL_ADC_GetValue(pin); // ����ȡ����ADCֵ�������adc
	return adc_value*3.3/4096; // 12λADC��2^12=4096
}
