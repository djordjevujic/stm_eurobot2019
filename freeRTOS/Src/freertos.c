/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * File Name          : freertos.c
 * Description        : Code for freertos applications
 ******************************************************************************
 * This notice applies to any and all portions of this file
 * that are not between comment pairs USER CODE BEGIN and
 * USER CODE END. Other portions of this file, whether
 * inserted by the user or by software development tools
 * are owned by their respective copyright owners.
 *
 * Copyright (c) 2019 STMicroelectronics International N.V.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted, provided that the following conditions are met:
 *
 * 1. Redistribution of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of STMicroelectronics nor the names of other
 *    contributors to this software may be used to endorse or promote products
 *    derived from this software without specific written permission.
 * 4. This software, including modifications and/or derivative works of this
 *    software, must execute solely and exclusively on microcontroller or
 *    microprocessor devices manufactured by or for STMicroelectronics.
 * 5. Redistribution and use of this software other than as permitted under
 *    this license is void and will automatically terminate your rights under
 *    this license.
 *
 * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
 * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT
 * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "usart.h"
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
/* USER CODE BEGIN Variables */
static volatile int16_t count = 0;

osThreadId Task3Handle;
/* USER CODE END Variables */
osThreadId defaultTaskHandle;
osThreadId Task2Handle;
osMutexId UartMutexHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
void Thread3(void const * argument);
/* USER CODE END FunctionPrototypes */

void RegulationTask(void const * argument);
void StartTask2(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
 * @brief  FreeRTOS initialization
 * @param  None
 * @retval None
 */
void MX_FREERTOS_Init(void) {
	/* USER CODE BEGIN Init */

	/* USER CODE END Init */

	/* Create the mutex(es) */
	/* definition and creation of UartMutex */
	osMutexDef(UartMutex);
	UartMutexHandle = osMutexCreate(osMutex(UartMutex));

	/* USER CODE BEGIN RTOS_MUTEX */
	/* add mutexes, ... */
	/* USER CODE END RTOS_MUTEX */

	/* USER CODE BEGIN RTOS_SEMAPHORES */
	/* add semaphores, ... */
	/* USER CODE END RTOS_SEMAPHORES */

	/* USER CODE BEGIN RTOS_TIMERS */
	/* start timers, add new ones, ... */
	/* USER CODE END RTOS_TIMERS */

	/* Create the thread(s) */
	/* definition and creation of defaultTask */
	osThreadDef(defaultTask, RegulationTask, osPriorityNormal, 0, 128);
	defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

	/* definition and creation of Task2 */
	osThreadDef(Task2, StartTask2, osPriorityIdle, 0, 128);
	Task2Handle = osThreadCreate(osThread(Task2), NULL);

	/* USER CODE BEGIN RTOS_THREADS */
	/* add threads, ... */
	osThreadDef(Task3, Thread3, osPriorityIdle, 0, 128);
	Task3Handle = osThreadCreate(osThread(Task3), NULL);
	/* USER CODE END RTOS_THREADS */

	/* USER CODE BEGIN RTOS_QUEUES */
	/* add queues, ... */
	/* USER CODE END RTOS_QUEUES */
}

/* USER CODE BEGIN Header_RegulationTask */
/**
 * @brief  Function implementing the defaultTask thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_RegulationTask */
void RegulationTask(void const * argument) {
	/* USER CODE BEGIN RegulationTask */
	TickType_t xLastWakeTime;
	const TickType_t xFrequency = 1;

	uint8_t txData[20] = "Hello from Thread1\r\n";

    xLastWakeTime = xTaskGetTickCount();
	/* Infinite loop */
	for (;;) {

		count++;
//		if(count == 1)
//		{
//			HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
//		}

		if(count == 500)
		{
			HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
			count = 0;
		}

		vTaskDelayUntil(&xLastWakeTime, xFrequency);
        //xSemaphoreTake(UartMutexHandle, portMAX_DELAY);
		//HAL_UART_Transmit(&huart2, txData, 20, 5);
		//xSemaphoreGive(UartMutexHandle);

	}
	/* USER CODE END RegulationTask */
}

/* USER CODE BEGIN Header_StartTask2 */
/**
 * @brief Function implementing the Task2 thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartTask2 */
void StartTask2(void const * argument) {
	/* USER CODE BEGIN StartTask2 */

	uint8_t txData[20] = "Hello from Thread2\r\n";

	for (;;) {

		xSemaphoreTake(UartMutexHandle, portMAX_DELAY);
		HAL_UART_Transmit(&huart2, txData, 20, 5);
		xSemaphoreGive(UartMutexHandle);

		osDelay(1000);
	}
	/* USER CODE END StartTask2 */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
void Thread3(void const * argument) {

	uint8_t txData[20] = "Hello from Thread3\r\n";

	for (;;) {

		xSemaphoreTake(UartMutexHandle, portMAX_DELAY);
		HAL_UART_Transmit(&huart2, txData, 20, 5);
		xSemaphoreGive(UartMutexHandle);

		osDelay(1000);
	}
	/* USER CODE END StartTask2 */
}
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
