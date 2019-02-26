/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 ** This notice applies to any and all portions of this file
 * that are not between comment pairs USER CODE BEGIN and
 * USER CODE END. Other portions of this file, whether
 * inserted by the user or by software development tools
 * are owned by their respective copyright owners.
 *
 * COPYRIGHT(c) 2019 STMicroelectronics
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *   1. Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright notice,
 *      this list of conditions and the following disclaimer in the documentation
 *      and/or other materials provided with the distribution.
 *   3. Neither the name of STMicroelectronics nor the names of its contributors
 *      may be used to endorse or promote products derived from this software
 *      without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "parser.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define DEG_TO_INC_COEFF 9.977333
#define INC_TO_DEG_COEFF 0.1002271816
#define ERROR_TOLERANCE_INC 10
#define DEFAULT_ENC_COUNTER_VALUE 20000
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
static volatile int32_t position = 0;
static volatile int32_t error = 0;
static volatile int32_t reference = 3592;
static volatile int32_t temp = 0;
static const float Ts = 0.001; //Sampling period
static volatile float kp = 2.0;
static volatile float Ti = 1000.0;
static volatile float ki = 0;
static float angle = 720;
static const unsigned int pwm_period = 1050;
static int32_t u = 0;
static int32_t ui = 0; //Integration part of control
static uint8_t finish_flag = 0;
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
int main(void) {
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
	MX_USART2_UART_Init();
	MX_TIM1_Init();
	MX_TIM2_Init();
	MX_TIM3_Init();

	/* USER CODE BEGIN 2 */
	HAL_TIM_Encoder_Start(&htim1, TIM_CHANNEL_1);
	HAL_TIM_Encoder_Start(&htim1, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);

	// Set Encoder count to middle value
	TIM1->CNT = DEFAULT_ENC_COUNTER_VALUE;
	temp = TIM1->CNT;

	ki = 0;	//kp / Ti;

	//Enable regulation interrupt
	HAL_TIM_Base_Start_IT(&htim3);
	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	char p[30] = "";
	char a[30] = "";
	char c[30] = "";

	uint8_t uart_receive_buff[50];
	uint8_t uart_rec_buff_offset = 0;
	uint8_t msg[20] = ""; // @TODO Delete later

	reference = (int32_t) (angle * DEG_TO_INC_COEFF);

	for (int i = 0; i < 50; i++) {
		uart_receive_buff[i] = 0;
	}

	while (1) {

		/*
		 HAL_UART_Transmit(&huart2, &primer, sizeof(primer), 50);
		 HAL_Delay(1000);
		 */

		snprintf(p, sizeof(p), "\n\ru: %d", u);
		HAL_UART_Transmit(&huart2, p, sizeof(p), 50);
		snprintf(a, sizeof(a), "\n\r%f", ((float) error * INC_TO_DEG_COEFF));
		HAL_UART_Transmit(&huart2, a, sizeof(a), 50);
		snprintf(c, sizeof(c), "\n\rcnt: %d", TIM1->CNT);
		HAL_UART_Transmit(&huart2, c, sizeof(c), 50);

		HAL_Delay(500);
		/*
		 if (HAL_UART_Receive(&huart2, uart_receive_buff + uart_rec_buff_offset,
		 1, 20) == HAL_OK) {

		 HAL_UART_Transmit(&huart2, uart_receive_buff + uart_rec_buff_offset,
		 1, 50);

		 if (uart_receive_buff[uart_rec_buff_offset] == '\n') {

		 uart_receive_buff[uart_rec_buff_offset] = '\0';
		 //msg_parse(uart_receive_buff);
		 strcpy(msg, uart_receive_buff);
		 uart_rec_buff_offset = 0;
		 //HAL_UART_Transmit(&huart2, msg, sizeof(msg), 50);

		 }

		 else
		 uart_rec_buff_offset++;
		 }
		 */
		//HAL_UART_Transmit(&huart2, p, sizeof(p), 50);

		//HAL_UART_Transmit(&huart2, p, sizeof(p), 50);
		//__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_1, 0);
		//test this
		/*
		 cnts = __HAL_TIM_GET_COUNTER(&htim1);

		 if (cnts != cnts_old) {
		 snprintf(p, sizeof(p), "cnts: %d\n\r", cnts);

		 HAL_UART_Transmit(&huart2, p, strlen(p), 50);


		 cnts_old = cnts;
		 }
		 HAL_Delay(200);
		 */

		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */
	}
	/* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void) {
	RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
	RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };

	/**Configure the main internal regulator output voltage
	 */
	__HAL_RCC_PWR_CLK_ENABLE()
	;
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);
	/**Initializes the CPU, AHB and APB busses clocks
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
	RCC_OscInitStruct.PLL.PLLM = 16;
	RCC_OscInitStruct.PLL.PLLN = 336;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
	RCC_OscInitStruct.PLL.PLLQ = 7;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler();
	}
	/**Initializes the CPU, AHB and APB busses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
			| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) {
		Error_Handler();
	}
}

/* USER CODE BEGIN 4 */

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {

	static char msg[20] = "";

	if (htim->Instance == TIM3) {
		//HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);

		int32_t count = 0;

		//count = __HAL_TIM_GET_COUNTER(&htim1);
		//__HAL_TIM_SET_COUNTER(&htim1, 0);

		count = __HAL_TIM_GET_COUNTER(&htim1);
		__HAL_TIM_SET_COUNTER(&htim1, DEFAULT_ENC_COUNTER_VALUE);

		count = count - DEFAULT_ENC_COUNTER_VALUE;

		//TIM1->CNT = 0;

		position += count;

		//snprintf(p, sizeof(p), "\n\r%d", error);
		//HAL_UART_Transmit(&huart2, p, sizeof(p), 50);

		error = reference - position;
		/*

		 */
		ui = ui + (int32_t) (ki * Ts * error);

		if (ui > 200)
			ui = 200;

		u = (int32_t) (kp * error) + ui;

		if (u > 800)
			u = 800;

		if (u < 30) {
			u = 0;
			finish_flag = 1;
		}

		// Direction
		if (u < 0)
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET);
		else if (u > 0)
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);
		//if (abs(error) > ERROR_TOLERANCE_INC)
		__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_1, abs(u));
		//else
		//	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_1, 0);

	}
}

//static inline uint32_t abs
/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) {
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
