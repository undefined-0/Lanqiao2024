#include "my_adc.h"

double getADC(ADC_HandleTypeDef *pin) // 读取ADC值的函数，参数是ADC的引脚
{
	uint16_t adc_value; // 新建变量用来存ADC值
	HAL_ADC_Start(pin); // 开启ADC
	adc_value = HAL_ADC_GetValue(pin); // 将读取到的ADC值存入变量adc
	return adc_value*3.3/4096; // 12位ADC，2^12=4096
}
