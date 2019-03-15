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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */     
#include "tim.h"
#include "usart.h"
#include "message.h"
#include "regulation.h"
#include "limit_switch.h"
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
osThreadId Task3Handle;
/* USER CODE END Variables */
osThreadId defaultTaskHandle;
osThreadId MessageReadTaskHandle;
osThreadId RegDataSendTaskHandle;
osThreadId LimitSwitchTaskHandle;
osMutexId UartMutexHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
void Thread3(void const * argument);
void threadMessageRead(void const * argument);
/* USER CODE END FunctionPrototypes */

void RegulationTask(void const * argument);
void threadMessageRead(void const * argument);
void TestDataSend(void const * argument);
void LimitSwitchEXINT(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
	regulator_init();
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
  osThreadDef(defaultTask, RegulationTask, osPriorityNormal, 0, 256);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* definition and creation of MessageReadTask */
  osThreadDef(MessageReadTask, threadMessageRead, osPriorityIdle, 0, 256);
  MessageReadTaskHandle = osThreadCreate(osThread(MessageReadTask), NULL);

  /* definition and creation of RegDataSendTask */
  osThreadDef(RegDataSendTask, TestDataSend, osPriorityIdle, 0, 512);
  RegDataSendTaskHandle = osThreadCreate(osThread(RegDataSendTask), NULL);

  /* definition and creation of LimitSwitchTask */
  osThreadDef(LimitSwitchTask, LimitSwitchEXINT, osPriorityHigh, 0, 128);
  LimitSwitchTaskHandle = osThreadCreate(osThread(LimitSwitchTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
	/* add threads, ... */

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
void RegulationTask(void const * argument)
{

  /* USER CODE BEGIN RegulationTask */
	TickType_t xLastWakeTime;
	const TickType_t xFrequency = 1;

	uint8_t txData[20] = "Hello from Thread1\r\n";

	//__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_1, 0); //@NOTE: Check channel if changing board


	xLastWakeTime = xTaskGetTickCount();
	/* Infinite loop */
	for (;;)
	{

		regulator();
//		count++;
//		if(count == 800)
//		{
//			//HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
//			count = 0;
//		}
//		__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_1, count);

		//__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_1, 600);

		vTaskDelayUntil(&xLastWakeTime, xFrequency);
		//xSemaphoreTake(UartMutexHandle, portMAX_DELAY);
		//HAL_UART_Transmit(&huart2, txData, 20, 5);
		//xSemaphoreGive(UartMutexHandle);

	}
  /* USER CODE END RegulationTask */
}

/* USER CODE BEGIN Header_threadMessageRead */
/**
* @brief Function implementing the MessageReadTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_threadMessageRead */
void threadMessageRead(void const * argument)
{
  /* USER CODE BEGIN threadMessageRead */
  /* Infinite loop */

	for(;;)
  {
		message_read();
		//osDelay(1000);
  }
  /* USER CODE END threadMessageRead */
}

/* USER CODE BEGIN Header_TestDataSend */
/**
* @brief Function implementing the RegDataSendTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_TestDataSend */
void TestDataSend(void const * argument)
{
  /* USER CODE BEGIN TestDataSend */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1000);
  }
  /* USER CODE END TestDataSend */
}

/* USER CODE BEGIN Header_LimitSwitchEXINT */
/**
* @brief Function implementing the LimitSwitchTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_LimitSwitchEXINT */
void LimitSwitchEXINT(void const * argument)
{
  /* USER CODE BEGIN LimitSwitchEXINT */
  /* Infinite loop */
	char msg[25] = "Hello from LSWEXINT\n\r";
  for(;;)
  {
    vTaskSuspend(NULL);
		limit_switch_handle();
    xSemaphoreTake(getUartMutex(), portMAX_DELAY);
		HAL_UART_Transmit(&huart2, msg, sizeof(msg), 5);
		xSemaphoreGive(getUartMutex());
  }
  /* USER CODE END LimitSwitchEXINT */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

osMutexId getUartMutex(void)
{
	return UartMutexHandle;
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	BaseType_t checkIfYieldRequired;

	set_lsw_flag(GPIO_Pin);

	checkIfYieldRequired = xTaskResumeFromISR(LimitSwitchTaskHandle);
	portYIELD_FROM_ISR(checkIfYieldRequired);
}

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
