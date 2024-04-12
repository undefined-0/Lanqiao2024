#include "led.h"

// LED共八个，引脚为PC8-PC15
// 锁存器引脚为PD2

void LED_Init(void)
{
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_All,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET); // 高电平解锁
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET); // 低电平锁存
}

void LED_Display(uint8_t i) // 参数转换为8位二进制后 1为亮0为灭
{
	LED_Init();
	HAL_GPIO_WritePin(GPIOC,i<<8,GPIO_PIN_RESET); // LED为共阳极  低电平时点亮
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET);
}
