/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef enum {
    WAITING_FOR_HEADER,
	WAITING_FOR_DATA,
    RESPONDING
} fsm;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define DATA_LEN 3
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;

UART_HandleTypeDef huart4;

/* USER CODE BEGIN PV */
volatile uint16_t pot_values[4];
volatile bool switch_states[8];
volatile bool LED_states[8];
volatile bool SEG_states[11];
const uint8_t adcChannelCount = 4;
const uint8_t header = 0xaa;
uint8_t UART4_rxBuffer[DATA_LEN] = {0};
char message[] = "Transmission\r\n"; /* Message to be transmitted through UART */

fsm state = WAITING_FOR_HEADER;

uint8_t cmd_id = 0x00;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_ADC1_Init(void);
static void MX_UART4_Init(void);
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
  MX_DMA_Init();
  MX_ADC1_Init();
  MX_UART4_Init();
  /* USER CODE BEGIN 2 */
  HAL_UART_Transmit(&huart4, (uint8_t *)message, strlen(message), 10);
  HAL_UART_Receive_IT(&huart4, UART4_rxBuffer, 1);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	HAL_ADC_Start_DMA(&hadc1, (uint32_t *)pot_values, adcChannelCount);
	// Get toggle states
	switch_states[0] = HAL_GPIO_ReadPin(TOG_0_GPIO_Port, TOG_0_Pin);
	switch_states[1] = HAL_GPIO_ReadPin(TOG_1_GPIO_Port, TOG_1_Pin);
	switch_states[2] = HAL_GPIO_ReadPin(TOG_2_GPIO_Port, TOG_2_Pin);
	switch_states[3] = HAL_GPIO_ReadPin(TOG_3_GPIO_Port, TOG_3_Pin);
	// Get button states (inverted because normally high)
	switch_states[4] = !HAL_GPIO_ReadPin(SW_0_GPIO_Port, SW_0_Pin);
	switch_states[5] = !HAL_GPIO_ReadPin(SW_1_GPIO_Port, SW_1_Pin);
	switch_states[6] = !HAL_GPIO_ReadPin(SW_2_GPIO_Port, SW_2_Pin);
	switch_states[7] = !HAL_GPIO_ReadPin(SW_3_GPIO_Port, SW_3_Pin);
	// Update LED states
	HAL_GPIO_WritePin(LED_0_GPIO_Port, LED_0_Pin, LED_states[0]);
	HAL_GPIO_WritePin(LED_1_GPIO_Port, LED_1_Pin, LED_states[1]);
	HAL_GPIO_WritePin(LED_2_GPIO_Port, LED_2_Pin, LED_states[2]);
	HAL_GPIO_WritePin(LED_3_GPIO_Port, LED_3_Pin, LED_states[3]);
	HAL_GPIO_WritePin(LED_4_GPIO_Port, LED_4_Pin, LED_states[4]);
	HAL_GPIO_WritePin(LED_5_GPIO_Port, LED_5_Pin, LED_states[5]);
	HAL_GPIO_WritePin(LED_6_GPIO_Port, LED_6_Pin, LED_states[6]);
	HAL_GPIO_WritePin(LED_7_GPIO_Port, LED_7_Pin, LED_states[7]);
	// Update Segment Display states
	HAL_GPIO_WritePin(SEG_G0_GPIO_Port, SEG_G0_Pin, SEG_states[0]);
	HAL_GPIO_WritePin(SEG_G1_GPIO_Port, SEG_G1_Pin, SEG_states[1]);
	HAL_GPIO_WritePin(SEG_G2_GPIO_Port, SEG_G2_Pin, SEG_states[2]);
	HAL_GPIO_WritePin(SEG_A_GPIO_Port, SEG_A_Pin, SEG_states[3]);
	HAL_GPIO_WritePin(SEG_B_GPIO_Port, SEG_B_Pin, SEG_states[4]);
	HAL_GPIO_WritePin(SEG_C_GPIO_Port, SEG_C_Pin, SEG_states[5]);
	HAL_GPIO_WritePin(SEG_D_GPIO_Port, SEG_D_Pin, SEG_states[6]);
	HAL_GPIO_WritePin(SEG_E_GPIO_Port, SEG_E_Pin, SEG_states[7]);
	HAL_GPIO_WritePin(SEG_F_GPIO_Port, SEG_F_Pin, SEG_states[8]);
	HAL_GPIO_WritePin(SEG_G_GPIO_Port, SEG_G_Pin, SEG_states[9]);
	HAL_GPIO_WritePin(SEG_DP_GPIO_Port, SEG_DP_Pin, SEG_states[10]);
	// Test transmit
	// sprintf(buffer, "P0=%d\tP1=%d\tP2=%d\tP3=%d\t", potValues[0], potValues[1], potValues[2], potValues[3]);
	// HAL_UART_Transmit(&huart4, (uint8_t *)buffer, strlen(buffer), 10);
	// sprintf(buffer, "T0=%d\tT1=%d\tT2=%d\tT3=%d\t", toggleStates[0], toggleStates[1], toggleStates[2], toggleStates[3]);
	// HAL_UART_Transmit(&huart4, (uint8_t *)buffer, strlen(buffer), 10);
	// sprintf(buffer, "B0=%d\tB1=%d\tB2=%d\tB3=%d\r\n", buttonStates[0], buttonStates[1], buttonStates[2], buttonStates[3]);
	// HAL_UART_Transmit(&huart4, (uint8_t *)buffer, strlen(buffer), 10);
//	HAL_UART_Transmit(&huart4, (uint8_t *)&header, sizeof(header), 10);
//	HAL_UART_Transmit(&huart4, (uint8_t *)&potValues, sizeof(potValues), 10);
//	HAL_UART_Transmit(&huart4, (uint8_t *)&toggleStates, sizeof(toggleStates), 10);
//	HAL_UART_Transmit(&huart4, (uint8_t *)&buttonStates, sizeof(buttonStates), 10);

	switch(state) {
	case RESPONDING: // handle command
		// handle received data
		for (int i = 7; i >= 0; i--) {
			LED_states[i] = (UART4_rxBuffer[0] >> i) & 1;
		}
		for (int i = 7; i >= 0; i--) {
			SEG_states[i] = (UART4_rxBuffer[1] >> i) & 1;
		}
		for (int i = 2; i >= 0; i--) {
			SEG_states[i+8] = (UART4_rxBuffer[2] >> i) & 1;
		}
		// convert switch bools to unit8_t
		uint8_t switch_states_uint8 = 0;
		for (int i = 0; i < 8; i++) {
			if (switch_states[i]) {
				switch_states_uint8 |= (1 << i);
			}
		}
		// send reply
		HAL_UART_Transmit(&huart4, (uint8_t *)&header, sizeof(header), 10);
		HAL_UART_Transmit(&huart4, (uint8_t *)&pot_values, sizeof(pot_values), 10);
		HAL_UART_Transmit(&huart4, (uint8_t *)&switch_states_uint8, sizeof(switch_states_uint8), 10);
		// enable Rx callback
		state = WAITING_FOR_HEADER;
		HAL_UART_Receive_IT(&huart4, UART4_rxBuffer, 1);
		break;
	default:
		break;
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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.ScanConvMode = ENABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 4;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_0;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_1;
  sConfig.Rank = 2;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_2;
  sConfig.Rank = 3;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_3;
  sConfig.Rank = 4;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief UART4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_UART4_Init(void)
{

  /* USER CODE BEGIN UART4_Init 0 */

  /* USER CODE END UART4_Init 0 */

  /* USER CODE BEGIN UART4_Init 1 */

  /* USER CODE END UART4_Init 1 */
  huart4.Instance = UART4;
  huart4.Init.BaudRate = 115200;
  huart4.Init.WordLength = UART_WORDLENGTH_8B;
  huart4.Init.StopBits = UART_STOPBITS_1;
  huart4.Init.Parity = UART_PARITY_NONE;
  huart4.Init.Mode = UART_MODE_TX_RX;
  huart4.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart4.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart4) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN UART4_Init 2 */

  /* USER CODE END UART4_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA2_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA2_Stream0_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LED_0_Pin|LED_1_Pin|LED_2_Pin|LED_3_Pin
                          |LED_4_Pin|LED_5_Pin|LED_6_Pin|LED_7_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOG, SEG_G0_Pin|SEG_G1_Pin|SEG_A_Pin|SEG_F_Pin
                          |SEG_B_Pin|SEG_G2_Pin|SEG_G_Pin|SEG_C_Pin
                          |SEG_DP_Pin|SEG_D_Pin|SEG_E_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : SW_0_Pin SW_1_Pin SW_2_Pin SW_3_Pin
                           TOG_0_Pin TOG_1_Pin TOG_2_Pin TOG_3_Pin */
  GPIO_InitStruct.Pin = SW_0_Pin|SW_1_Pin|SW_2_Pin|SW_3_Pin
                          |TOG_0_Pin|TOG_1_Pin|TOG_2_Pin|TOG_3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pin : PA0 */
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : LED_0_Pin LED_1_Pin LED_2_Pin LED_3_Pin
                           LED_4_Pin LED_5_Pin LED_6_Pin LED_7_Pin */
  GPIO_InitStruct.Pin = LED_0_Pin|LED_1_Pin|LED_2_Pin|LED_3_Pin
                          |LED_4_Pin|LED_5_Pin|LED_6_Pin|LED_7_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : SEG_G0_Pin SEG_G1_Pin SEG_A_Pin SEG_F_Pin
                           SEG_B_Pin SEG_G2_Pin SEG_G_Pin SEG_C_Pin
                           SEG_DP_Pin SEG_D_Pin SEG_E_Pin */
  GPIO_InitStruct.Pin = SEG_G0_Pin|SEG_G1_Pin|SEG_A_Pin|SEG_F_Pin
                          |SEG_B_Pin|SEG_G2_Pin|SEG_G_Pin|SEG_C_Pin
                          |SEG_DP_Pin|SEG_D_Pin|SEG_E_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	switch(state) {
	case WAITING_FOR_HEADER:
		// check for recognised header byte
		switch(UART4_rxBuffer[0]) {
		case 0xaa: // recognised header
			cmd_id = UART4_rxBuffer[0];
			HAL_UART_Receive_IT(&huart4, UART4_rxBuffer, DATA_LEN); // now wait for data
			state = WAITING_FOR_DATA;
			break;
		default: // no recognised header, wait for next byte
			HAL_UART_Receive_IT(&huart4, UART4_rxBuffer, 1);
			break;
		}
		break;
	case WAITING_FOR_DATA: // data received... handle and reply
		state = RESPONDING;
		break;
	default:
		break;
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
