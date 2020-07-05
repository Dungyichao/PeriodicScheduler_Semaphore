

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "spi.h"
#include "tim.h"
#include "gpio.h"
#include "st7735.h"
#include "fonts.h"
#include "osKernel.h"

void SystemClock_Config(void);

#define		QUANTA		50   //30 ms

int32_t semaphore0,semaphore1,semaphore2;
int32_t count0, count1, count2;
void Task0(void)
{
	while(1)
	{
    //osSignalWait(&semaphore1);
		//osSignalSet(&semaphore0);
		osSignalWait(&semaphore1);

		count0++;
		//ST7735_DrawString(3,5,"Task0",ST7735_GREEN);
    osSignalSet(&semaphore0);
	
	}
}

void Task1(void)
{
	while(1)
	{
		
		//osSignalWait(&semaphore2);
		//osSignalSet(&semaphore1);
		osSignalWait(&semaphore0);

		count1++;
		//ST7735_DrawString(3,7,"Task1",ST7735_BLUE);
		osSignalSet(&semaphore1);
	}
}

void Task2(void)
{
	while(1)
	{
		//osSignalWait(&semaphore3);
//		osSignalSet(&semaphore2);
//		osSignalWait(&semaphore0);
//		osSignalWait(&semaphore1);
//		count2++;
		//ST7735_DrawString(3,9,"Task2",ST7735_RED);
    //osSignalSet(&semaphore1);
	}
}

int main(void)
{
	osSemaphoreInit(&semaphore0,0);
  osSemaphoreInit(&semaphore1,0);
	osSemaphoreInit(&semaphore2,0);
  
  HAL_Init();

 
  SystemClock_Config();

  
  MX_GPIO_Init();
  MX_SPI1_Init();
  MX_TIM2_Init();
	
	HAL_TIM_Base_Start_IT(&htim2);  // start the TIM2 timer
	
	SysTick->CTRL =0;  //disable SysTick Exception, or the TIM2 will be disturbed, LCD could not be initialized
	
	ST7735_Init();
  ST7735_FillScreen(ST7735_BLACK);
  ST7735_DrawString(1, 0, "CPU=", ST7735_ORANGE);
	
	osKernelInit();
  osKernelAddThreads(&Task0,&Task1,&Task2);
  osKernelLaunch(QUANTA);
  
  while (1)
  {
   
  }
  
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
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 64;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV4;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
