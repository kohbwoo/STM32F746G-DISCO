/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
void Segment_Test(void);
int Segment(int SegmentNum, int PrintNum);
/**
 * @brief  The application entry point.
 * @retval int
 */
int i = 0;
int List_Of_Segments[4][4] =
	{
			{0,0,0,1},
			{0,0,1,0},
			{0,1,0,0},
			{1,0,0,0}
	};
int List_Of_Segment_Info[10][8] =
	{
		{1,1,0,0,0,0,0,0}, //0
		{1,1,1,1,1,0,0,1}, //1
		{1,0,1,0,0,1,0,0}, //2
		{1,0,1,1,0,0,0,0}, //3
		{1,0,0,1,1,0,0,1}, //4
		{1,0,0,1,0,0,1,0}, //5
		{1,0,0,0,0,0,1,0}, //6
		{1,1,0,1,1,0,0,0}, //7
		{1,0,0,0,0,0,0,0}, //8
		{1,0,0,1,1,0,0,0} //9
		//List_Of_Segment[10][0]은 Dot
	};

int main(void) {


	/* Enable I-Cache---------------------------------------------------------*/
	SCB_EnableICache();
	/* Enable D-Cache---------------------------------------------------------*/
	SCB_EnableDCache();
	/* MCU Configuration--------------------------------------------------------*/
	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();


	/* Configure the system clock */
	SystemClock_Config();

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	/*
	    A
	   ---
	F |   | B
	  | G |
	   ---
	E |   | C
	  |   |
	   ---
	    D
	pinA = 2 = PG6  = (GPIOG, GPIO_PIN_6)
	pinB = 3 = PB4  = (GPIOB, GPIO_PIN_4)
	pinC = 4 = PG7  = (GPIOG, GPIO_PIN_7)
	pinD = 5 = PI0  = (GPIOI, GPIO_PIN_0)
	pinE = 6 = PH6  = (GPIOH, GPIO_PIN_6)
	pinF = 7 = PI3  = (GPIOI, GPIO_PIN_3)
	pinG = 8 = PI2  = (GPIOI, GPIO_PIN_2)
	D1 = 9   = PA15 = (GPIOA, GPIO_PIN_15)
	D2 = 10  = PA8  = (GPIOA, GPIO_PIN_8)
	D3 = 11  = PB15 = (GPIOB, GPIO_PIN_15)
	D4 = 12  = PB14 = (GPIOB, GPIO_PIN_14)
	*/

	int delaytime = 1;
	int addr[4] = {0,0,0,0};
	int input = 1234; //인풋

	addr[0] = input / 1000;
	addr[1] = input % 1000 / 100;
	addr[2] = input % 100 / 10;
	addr[3] = input % 10;

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */

	while (1) {
		Segment(0, addr[0]);
		HAL_Delay(delaytime);
		Segment(1, addr[1]);
		HAL_Delay(delaytime);
		Segment(2, addr[2]);
		HAL_Delay(delaytime);
		Segment(3, addr[3]);
		HAL_Delay(delaytime);
	}
	/* USER CODE END 3 */
}

int Segment(int SegmentNum, int PrintNum) {

	//출력할 세그먼트 결정
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, List_Of_Segments[SegmentNum][3]);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, List_Of_Segments[SegmentNum][2]);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, List_Of_Segments[SegmentNum][1]);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, List_Of_Segments[SegmentNum][0]);

	//세그먼트의 출력 내용 결정
	HAL_GPIO_WritePin(GPIOG, GPIO_PIN_6, List_Of_Segment_Info[PrintNum][7]);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, List_Of_Segment_Info[PrintNum][6]);
	HAL_GPIO_WritePin(GPIOG, GPIO_PIN_7, List_Of_Segment_Info[PrintNum][5]);
	HAL_GPIO_WritePin(GPIOI, GPIO_PIN_0, List_Of_Segment_Info[PrintNum][4]);
	HAL_GPIO_WritePin(GPIOH, GPIO_PIN_6, List_Of_Segment_Info[PrintNum][3]);
	HAL_GPIO_WritePin(GPIOI, GPIO_PIN_3, List_Of_Segment_Info[PrintNum][2]);
	HAL_GPIO_WritePin(GPIOI, GPIO_PIN_2, List_Of_Segment_Info[PrintNum][1]);
	return 0;
}

void Segment_Test(void) {
	int delaytime = 1;
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
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void) {
	RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
	RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };

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
	RCC_OscInitStruct.PLL.PLLN = 432;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ = 9;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler();
	}
	/** Activate the Over-Drive mode
	 */
	if (HAL_PWREx_EnableOverDrive() != HAL_OK) {
		Error_Handler();
	}
	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
			| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7) != HAL_OK) {
		Error_Handler();
	}
}

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void) {
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOI_CLK_ENABLE();
	__HAL_RCC_GPIOH_CLK_ENABLE();
	__HAL_RCC_GPIOG_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4 | GPIO_PIN_14 | GPIO_PIN_15,
			GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15 | GPIO_PIN_8, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOI, GPIO_PIN_3 | GPIO_PIN_2 | GPIO_PIN_1 | GPIO_PIN_0,
			GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOG, GPIO_PIN_7 | GPIO_PIN_6, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOH, GPIO_PIN_6, GPIO_PIN_RESET);

	/*Configure GPIO pins : PB4 PB14 PB15 */
	GPIO_InitStruct.Pin = GPIO_PIN_4 | GPIO_PIN_14 | GPIO_PIN_15;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/*Configure GPIO pins : PA15 PA8 */
	GPIO_InitStruct.Pin = GPIO_PIN_15 | GPIO_PIN_8;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/*Configure GPIO pins : PI3 PI2 PI1 PI0 */
	GPIO_InitStruct.Pin = GPIO_PIN_3 | GPIO_PIN_2 | GPIO_PIN_1 | GPIO_PIN_0;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOI, &GPIO_InitStruct);

	/*Configure GPIO pins : PG7 PG6 */
	GPIO_InitStruct.Pin = GPIO_PIN_7 | GPIO_PIN_6;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

	/*Configure GPIO pin : PH6 */
	GPIO_InitStruct.Pin = GPIO_PIN_6;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) {
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1) {
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
