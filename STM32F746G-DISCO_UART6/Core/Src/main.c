/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

TIM_HandleTypeDef htim3;

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart6;

/* USER CODE BEGIN PV */
uint8_t rx1_data;
uint8_t rx6_data;
unsigned char UART_Text_Input[7] = "Input: ";
unsigned char UART_Text_Time[7] = "Time: ";
unsigned char UART_Text_Error[5] = "ERROR";
unsigned char UART1_Text_Start[59] = "This is Uart 1\n\r";
unsigned char UART6_Text_Start[59] = "This is Uart 6\n\r";

unsigned char Enter[2] = "\n\r";
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM3_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_USART6_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
unsigned int Loop_Count = 0;
void Int_To_Str(int num, unsigned char *data);
unsigned char UART1_Input = 0;
unsigned char UART6_Input = 0; // 인터럽트 입력 여부 구별
unsigned char Segment_Test(unsigned short delaytime);
unsigned char Segment_Select(unsigned char SegmentNum, unsigned char PrintNum);
unsigned char Num_Select(unsigned char PrintNumx16);
unsigned short input = 0; //인풋///////////////////////////////////////////////////////////////////
unsigned char addr[4] = { 0, 0, 0, 0 };


void Line_Change1(void) {
	for(int i = 0; i<3; i++){
		HAL_UART_Transmit(&huart1, &Enter[i], 1, 10);
	}

}

void Line_Change6(void) {
	for(int i = 0; i<3; i++){
		HAL_UART_Transmit(&huart6, &Enter[i], 1, 10);
	}

}


void UART6_Start(void) {
	for (int i = 0; i <16; i++) {
		HAL_UART_Transmit(&huart6, &UART6_Text_Start[i], 1, 10);
	}
	Line_Change6();
}
void UART1_Start(void) {
	for (int i = 0; i <16; i++) {
		HAL_UART_Transmit(&huart1, &UART1_Text_Start[i], 1, 10);
	}
	Line_Change1();
}

unsigned char UART6_Print() {
	if (rx6_data == 84 || rx6_data == 116) { //UART 입력이 T 또는 t인경우 실행
		for (int i = 0; i < 7; i++) {
			HAL_UART_Transmit(&huart6, &UART_Text_Input[i], 1, 10);
		}
		HAL_UART_Transmit(&huart6, &rx6_data, 1, 10);
		Line_Change6();

		for (int i = 0; i < 7; i++) {
			HAL_UART_Transmit(&huart6, &UART_Text_Time[i], 1, 10);
		}

		unsigned char tmplist[4];

		for (int i = 0; i < 4; i++) {
			tmplist[i] = addr[i] + 48;
			HAL_UART_Transmit(&huart6, &tmplist[i], 1, 10);
		}
		Line_Change6();

	} else {
		for (int i = 0; i < 7; i++) {
			HAL_UART_Transmit(&huart6, &UART_Text_Input[i], 1, 10);
		}
		HAL_UART_Transmit(&huart6, &rx6_data, 1, 10);
		Line_Change6();
	}
	return 0;

}



unsigned char UART1_Print() {
	if (rx1_data == 84 || rx1_data == 116) { //UART 입력이 T 또는 t인경우 실행
		for (int i = 0; i < 7; i++) {
			HAL_UART_Transmit(&huart1, &UART_Text_Input[i], 1, 10);
		}
		HAL_UART_Transmit(&huart1, &rx1_data, 1, 10);
		Line_Change1();

		for (int i = 0; i < 7; i++) {
			HAL_UART_Transmit(&huart1, &UART_Text_Time[i], 1, 10);
		}

		unsigned char tmplist[4];

		for (int i = 0; i < 4; i++) {
			tmplist[i] = addr[i] + 48;
			HAL_UART_Transmit(&huart1, &tmplist[i], 1, 10);
		}
		Line_Change1();

	} else {
		for (int i = 0; i < 7; i++) {
			HAL_UART_Transmit(&huart1, &UART_Text_Input[i], 1, 10);
		}
		HAL_UART_Transmit(&huart1, &rx1_data, 1, 10);
		Line_Change1();
	}
	return 0;

}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	if (huart->Instance == USART1) {
		HAL_UART_Receive_IT(&huart1, &rx1_data, 1);
		UART1_Input = 1;

	}

	if (huart->Instance == USART6) {
		HAL_UART_Receive_IT(&huart6, &rx6_data, 1);
		UART6_Input = 1;

	}

}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) //타이머 인터럽트 코드
{
	if (htim->Instance == TIM3) {

		HAL_GPIO_TogglePin(GPIOI, GPIO_PIN_1);
		input = input + 1;
	}
}

unsigned char Num_Select(unsigned char PrintNumx16) {
	if (PrintNumx16 & 0x40) {
		HAL_GPIO_WritePin(GPIOI, GPIO_PIN_2, 1);
	} else {
		HAL_GPIO_WritePin(GPIOI, GPIO_PIN_2, 0);
	}
	if (PrintNumx16 & 0x20) {
		HAL_GPIO_WritePin(GPIOI, GPIO_PIN_3, 1);
	} else {
		HAL_GPIO_WritePin(GPIOI, GPIO_PIN_3, 0);
	}
	if (PrintNumx16 & 0x10) {
		HAL_GPIO_WritePin(GPIOH, GPIO_PIN_6, 1);
	} else {
		HAL_GPIO_WritePin(GPIOH, GPIO_PIN_6, 0);
	}
	if (PrintNumx16 & 0x08) {
		HAL_GPIO_WritePin(GPIOI, GPIO_PIN_0, 1);
	} else {
		HAL_GPIO_WritePin(GPIOI, GPIO_PIN_0, 0);
	}
	if (PrintNumx16 & 0x04) {
		HAL_GPIO_WritePin(GPIOG, GPIO_PIN_7, 1);
	} else {
		HAL_GPIO_WritePin(GPIOG, GPIO_PIN_7, 0);
	}
	if (PrintNumx16 & 0x02) {
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, 1);
	} else {
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, 0);
	}
	if (PrintNumx16 & 0x01) {
		HAL_GPIO_WritePin(GPIOG, GPIO_PIN_6, 1);
	} else {
		HAL_GPIO_WritePin(GPIOG, GPIO_PIN_6, 0);
	}
	return 0;
}

unsigned char Segment_Select(unsigned char SegmentNumx16,
		unsigned char PrintNumx16) {
	//출력할 세그먼트 결정

	if (SegmentNumx16 & 0x08) {
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, 1);
	} else {
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, 0);
	}
	if (SegmentNumx16 & 0x04) {
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, 1);
	} else {
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, 0);
	}
	if (SegmentNumx16 & 0x02) {
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, 1);
	} else {
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, 0);
	}
	if (SegmentNumx16 & 0x01) {
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, 1);
	} else {
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, 0);
	}
	Num_Select(PrintNumx16);
	return 0;

}

unsigned char Segment_Test(unsigned short delaytime) {
//	unsigned short delaytime = 10;
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, 1);
	HAL_Delay(delaytime);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, 0);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, 1);
	HAL_Delay(delaytime);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, 0);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, 1);
	HAL_Delay(delaytime);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, 0);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, 1);
	HAL_Delay(delaytime);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, 0);
	return 0;
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* Enable I-Cache---------------------------------------------------------*/
  SCB_EnableICache();

  /* Enable D-Cache---------------------------------------------------------*/
  SCB_EnableDCache();

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
  MX_USART1_UART_Init();
  MX_USART6_UART_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_Base_Start_IT(&htim3); // 타이머 인터럽트 시작 디버그 할경우 주석처리 해야함
  	HAL_UART_Receive_IT(&huart6, &rx6_data, 1); // UART인터럽트 한바이트 들어오면 시작
  	HAL_UART_Receive_IT(&huart1, &rx1_data, 1); // UART인터럽트 한바이트 들어오면 시작
  	unsigned char List_Of_Segments[4] = { 0x01, 0x02, 0x04, 0x08 };
  	unsigned char List_Of_Segment_Info[10] = { 0xC0, 0xF9, 0xA4, 0xB0, 0x99,
  			0x92, 0x82, 0xD8, 0x80, 0x98 };
  	unsigned short delaytime = 1;
  	UART1_Start();
  	UART6_Start();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  Loop_Count++;
	  		if (input % 100 == 60) {
	  			input += 100;
	  			input -= 60;
	  		}
	  		if (input > 2359) {
	  			input = 0;
	  		}

	  		addr[0] = input / 1000;
	  		addr[1] = input % 1000 / 100;
	  		addr[2] = input % 100 / 10;
	  		addr[3] = input % 10;
	  		Segment_Select(List_Of_Segments[0], List_Of_Segment_Info[addr[0]]);
	  		HAL_Delay(delaytime);
	  		Segment_Select(List_Of_Segments[1], List_Of_Segment_Info[addr[1]]);
	  		HAL_Delay(delaytime);
	  		Segment_Select(List_Of_Segments[2], List_Of_Segment_Info[addr[2]]);
	  		HAL_Delay(delaytime);
	  		Segment_Select(List_Of_Segments[3], List_Of_Segment_Info[addr[3]]);
	  		HAL_Delay(delaytime);

	  		if (UART6_Input == 1) {
	  			UART6_Print();
	  			UART6_Input = 0;
	  		}

	  		if (UART1_Input == 1) {
				UART1_Print();
				UART1_Input = 0;
			}

    /* USER CODE END WHILE */
    /* USER CODE BEGIN 3 */
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
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 400;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Activate the Over-Drive mode
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_6) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USART1|RCC_PERIPHCLK_USART6;
  PeriphClkInitStruct.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
  PeriphClkInitStruct.Usart6ClockSelection = RCC_USART6CLKSOURCE_PCLK2;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 9999;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 9999;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 9600;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief USART6 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART6_UART_Init(void)
{

  /* USER CODE BEGIN USART6_Init 0 */

  /* USER CODE END USART6_Init 0 */

  /* USER CODE BEGIN USART6_Init 1 */

  /* USER CODE END USART6_Init 1 */
  huart6.Instance = USART6;
  huart6.Init.BaudRate = 9600;
  huart6.Init.WordLength = UART_WORDLENGTH_8B;
  huart6.Init.StopBits = UART_STOPBITS_1;
  huart6.Init.Parity = UART_PARITY_NONE;
  huart6.Init.Mode = UART_MODE_TX_RX;
  huart6.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart6.Init.OverSampling = UART_OVERSAMPLING_16;
  huart6.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart6.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart6) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART6_Init 2 */

  /* USER CODE END USART6_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOI_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4|GPIO_PIN_14|GPIO_PIN_15, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15|GPIO_PIN_8, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOI, GPIO_PIN_3|GPIO_PIN_2|GPIO_PIN_1|GPIO_PIN_0, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOG, GPIO_PIN_7|GPIO_PIN_6, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOH, GPIO_PIN_6, GPIO_PIN_RESET);

  /*Configure GPIO pins : PB4 PB14 PB15 */
  GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_14|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PA15 PA8 */
  GPIO_InitStruct.Pin = GPIO_PIN_15|GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PI3 PI2 PI1 PI0 */
  GPIO_InitStruct.Pin = GPIO_PIN_3|GPIO_PIN_2|GPIO_PIN_1|GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOI, &GPIO_InitStruct);

  /*Configure GPIO pins : PG7 PG6 */
  GPIO_InitStruct.Pin = GPIO_PIN_7|GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

  /*Configure GPIO pins : PF10 PF9 */
  GPIO_InitStruct.Pin = GPIO_PIN_10|GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

  /*Configure GPIO pin : PA0 */
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PH6 */
  GPIO_InitStruct.Pin = GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);

  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
