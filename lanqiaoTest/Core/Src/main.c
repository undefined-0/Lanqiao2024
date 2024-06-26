/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "string.h"
#include "led.h"
#include "lcd.h"
#include "interrupt.h"
#include "my_adc.h"
#include "i2c_hal.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
extern struct status key[4];
uint8_t view = 0; // 默认是在第一个界面
uint8_t pa6_duty = 10; // 引脚PA6的PWM波占空比（上电时默认为10%）
uint8_t pa7_duty = 10; // 引脚PA7的PWM波占空比（上电时默认为10%）
extern uint16_t frq_1,frq_2;
extern char rxdata[30]; // 用来储存已经处理好的数据
extern uint8_t rxdat; // 每次接收时用来存储字符的变量
extern uint8_t rx_pointer; // “指针”，确定我们写到哪个位置（因为是一位一位接收的）
void uart_rx_proc(void); // 串口接收处理函数
char temp[20]; // 用来串口发送的数组

char car_type[5];
char car_data[5];
char car_time[13];
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM3_Init();
  MX_TIM16_Init();
  MX_TIM17_Init();
  MX_TIM2_Init();
  MX_TIM4_Init();
  MX_ADC1_Init();
  MX_ADC2_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  //uint8_t i = 0x01;  
  //LED_Display(i);
  LED_Init();
  LCD_Init();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
/*	LCD_Clear(Blue);
    LCD_SetBackColor(Blue);
    LCD_SetTextColor(White);


    LCD_DrawLine(120, 0, 320, Horizontal);
    LCD_DrawLine(0, 160, 240, Vertical);
    HAL_Delay(1000);
    LCD_Clear(Blue);

    LCD_DrawRect(70, 210, 100, 100);
    HAL_Delay(1000);
    LCD_Clear(Blue);

    LCD_DrawCircle(120, 160, 80);
    HAL_Delay(1000);

    LCD_Clear(Blue);
    LCD_DisplayStringLine(Line4, (unsigned char *)"    Hello,world.   ");
    HAL_Delay(1000);

    LCD_SetBackColor(White);
    LCD_DisplayStringLine(Line0, (unsigned char *)"                    ");
    LCD_SetBackColor(Black);
    LCD_DisplayStringLine(Line1, (unsigned char *)"                    ");
    LCD_SetBackColor(Grey);
    LCD_DisplayStringLine(Line2, (unsigned char *)"                    ");
    LCD_SetBackColor(Blue);
    LCD_DisplayStringLine(Line3, (unsigned char *)"                    ");
    LCD_SetBackColor(Blue2);
    LCD_DisplayStringLine(Line4, (unsigned char *)"                    ");
    LCD_SetBackColor(Red);
    LCD_DisplayStringLine(Line5, (unsigned char *)"                    ");
    LCD_SetBackColor(Magenta);
    LCD_DisplayStringLine(Line6, (unsigned char *)"                    ");
    LCD_SetBackColor(Green);
    LCD_DisplayStringLine(Line7, (unsigned char *)"                    ");
    LCD_SetBackColor(Cyan);
    LCD_DisplayStringLine(Line8, (unsigned char *)"                    ");
    LCD_SetBackColor(Yellow);
    LCD_DisplayStringLine(Line9, (unsigned char *)"                    ");
	HAL_Delay(1000);*/
	LCD_Clear(Black);
//	char text[10];
	LCD_SetBackColor(Black);
    LCD_SetTextColor(White);
	
	HAL_TIM_Base_Start_IT(&htim4); // 开启中断（用来检测按键）
	
	HAL_TIM_PWM_Start(&htim16,TIM_CHANNEL_1); // 打开PWM输出（TIM16，通道1）
	HAL_TIM_PWM_Start(&htim17,TIM_CHANNEL_1); // 打开PWM输出（TIM17，通道1）
	
	HAL_TIM_IC_Start_IT(&htim2,TIM_CHANNEL_1); // 频率测量捕获定时器开启
	HAL_TIM_IC_Start_IT(&htim3,TIM_CHANNEL_1); // 频率测量捕获定时器开启
	
	HAL_UART_Receive_IT(&huart1,&rxdat,1); // 串口接收中断打开
	
	char text[10]; // 声明数组用来储存要显示在LCD屏幕上的文字
	sprintf(text,"       111 first"); // 上电默认显示第一个界面
    LCD_DisplayStringLine(Line1, (unsigned char *)text);
	
  while (1)
  {
	// if(key[0].key_short_flag == 1) // key0被短按
	// {
	// sprintf(text,"        key0");
	// LCD_DisplayStringLine(Line4, (unsigned char *)text);
		
	// key[0].key_short_flag = 0; // 将按键标志位清零，表明“已经执行过按下按键时想实现的功能了”
	// }
	
	// if(key[1].key_short_flag == 1) // key1被短按
	// {
	// sprintf(text,"        key1");
	// LCD_DisplayStringLine(Line4, (unsigned char *)text);
		
	// key[1].key_short_flag = 0; // 将按键标志位清零，表明“已经执行过按下按键时想实现的功能了”
	// }
	
	// if(key[3].key_long_flag == 1) // key3被长按
	// {
	// LED_Init();
	// LCD_Clear(Black);
	// key[3].key_long_flag = 0; // 将按键标志位清零，表明“已经执行过按下按键时想实现的功能了”
	// }
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	if(key[0].key_short_flag == 1) // key0被短按时切换界面
	{	
      LCD_Clear(Black); // 为避免上一个界面的文字残留，需要先清屏
      view = !view; // key0被按下时对view取反
      key[0].key_short_flag = 0; // 将按键标志位清零，表明“已经执行过按下按键时想实现的功能了”
		
	}
	if(key[1].key_short_flag == 1) // key1被短按
    {
         pa6_duty += 10;
           if(pa6_duty == 100)
             pa6_duty = 10;
         __HAL_TIM_SetCompare(&htim16,TIM_CHANNEL_1,pa6_duty); // HAL库函数，设置占空比
         key[1].key_short_flag = 0;
    }
	
    if(key[2].key_short_flag == 1) // key2被短按
    {
         pa7_duty += 10;
           if(pa7_duty == 100)
             pa7_duty = 10;
         __HAL_TIM_SetCompare(&htim17,TIM_CHANNEL_1,pa7_duty); // HAL库函数，设置占空比
         key[2].key_short_flag = 0;
	}
	
	    if(view == 0) // 若要显示第一个界面
        {
			//LCD_Clear(Black);
          sprintf(text,"       111 first");
          LCD_DisplayStringLine(Line1, (unsigned char *)text);
			//LCD_Clear(Black);
		  sprintf(text,"       FRQ1=%d",frq_1);
          LCD_DisplayStringLine(Line2, (unsigned char *)text);
			//LCD_Clear(Black);
		  sprintf(text,"       FRQ2=%d",frq_2);
          LCD_DisplayStringLine(Line4, (unsigned char *)text);
			
//			sprintf(text,"       ADC1=%.2f",getADC(&hadc1));
//          LCD_DisplayStringLine(Line6, (unsigned char *)text);
//			sprintf(text,"       ADC2=%.2f",getADC(&hadc2));
//          LCD_DisplayStringLine(Line8, (unsigned char *)text);
			
			uint16_t eep_temp = (eeprom_read(1)<<8) + eeprom_read(2); // 读取eeprom的数据，高八位左移8位+低八位
			sprintf(text,"  eeprom__frq=%d",eep_temp);
          LCD_DisplayStringLine(Line6, (unsigned char *)text);

        }
		
        if(view == 1) // 若要显示第二个界面
        {
			sprintf(text,"    PA6:%d",pa6_duty);
          LCD_DisplayStringLine(Line2, (unsigned char *)text);
			sprintf(text,"    PA7:%d",pa7_duty);
          LCD_DisplayStringLine(Line4, (unsigned char *)text);
        }
		 if(key[3].key_short_flag == 1) // key3被短按
		{
         uint8_t frq_high = frq_1>>8; // 频率是无符号16位，而eeprom芯片一个位只能存一个八位的数，所以要拆成高八位和低八位
		 uint8_t frq_low = frq_1&0xff; // 与0000 0000 1111 1111相与，取出低八位
			
			eeprom_write(1,frq_high);// 将两个数分别存入eeprom的两个区域里
			HAL_Delay(10); // 写入是需要时间的。如果不加延迟，很可能会写不进去
			eeprom_write(2,frq_low);
			
         key[3].key_short_flag = 0;
		}
		
		sprintf(temp,"frq = %d\r\n",frq_1);
		HAL_UART_Transmit(&huart1,(uint8_t *)temp,strlen(temp),50); // 串口发送数据
		
		if(rx_pointer!=0) // 串口接收数据（防止接收不完整问题）
		{
			int temp = rx_pointer;
			HAL_Delay(1);
			if(temp == rx_pointer)
				uart_rx_proc(); // 串口接收处理函数
		}
		
	}	


  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV3;
  RCC_OscInitStruct.PLL.PLLN = 20;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
		void uart_rx_proc() // 串口接收处理函数
	{
		 if(rx_pointer>0) // “指针”不在0的位置，说明接收到了
		 {
			 if(rx_pointer == 22) // 第十二届真题中发送的数据都是22位，由此即可判断是否接收成功
			 {
				 sscanf(rxdata,"%4s:%4s:%12s",car_type,car_data,car_time); // 把接收到的字符串拆开
			 }
			 else // 不是22位，说明接收错误
			 {
				 sprintf(temp,"error");
		HAL_UART_Transmit(&huart1,(uint8_t *)temp,strlen(temp),50); // 串口发送数据
			 }
			 rx_pointer = 0; // 接收完了，指针归位，准备接收下一次数据
			 memset(rxdata,0,30);
		 }
	}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
