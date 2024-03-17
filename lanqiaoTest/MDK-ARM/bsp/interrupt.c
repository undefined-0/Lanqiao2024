#include "interrupt.h"

/* 
根据原理图，
按键按下->引脚为低电平0
按键未按下->引脚为高电平1
*/

struct status key[4] = {{0,0,0},{0,0,0},{0,0,0},{0,0,0}};
// 四个按键->四个结构体变量
// 每个结构体变量的三个参数都被赋值为0

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) // 中断回调函数
{
	if(htim->Instance == TIM3) //判断是否是来自TIM3的中断
	{
		// 读取四个按键的状态（引脚电平）
		key[0].key_status = HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0); // PB0
		key[1].key_status = HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_1); // PB1
		key[2].key_status = HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_2); // PB2
		key[3].key_status = HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0); // PA0
		
		// 轮询
		for(uint8_t i=0; i<4; i++)
		{
			switch(key[i].step)
			{
				case 0: //判断按下
				{
					if(key[i].key_status == 0) // 通过if开始时的检测，发现按键可能被按下了（检测到引脚状态由1变0，但还不能确定是扰动还是按键真的被按下）
						key[i].step = 1; // 按键状态进行到第一步
				}
				break;
				
				case 1: // 消抖过程
				{
					if(key[i].key_status == 0) // 10ms后再检测引脚电平。如果还是低电平，就说明按键真的被按下了
					{	key[i].step = 2; // 按键状态进行到第二步
						key[i].key_flag = 1; // 按键真的被按下了
					}
					else key[i].step = 0; // 按键并没有被按下，只是受到了扰动。所以此时将按键状态退回到初始状态（将step赋值为0）。
				}
				break;
				
				case 2: // 判断松手
				{
					if(key[i].key_status == 1) // 引脚状态由0变1，说明按键被松开了
					{
						key[i].step = 0; // 将按键状态退回到初始状态（将step赋值为0）
					}
				}
				break;
			}
		} // 轮询结束，重新读取四个按键的引脚状态
	}
} // 判断按键的中断回调函数结束
