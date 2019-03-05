/*
 * message.c
 *
 *  Created on: Feb 28, 2019
 *      Author: djox
 */

//@TODO Message words handling
//@TODO Resolve WARNINGS!
#include <string.h>
#include <stdlib.h>

#include "FreeRTOS.h"
#include "task.h"

#include "main.h"
#include "message.h"
#include "cmsis_os.h"
#include "actuator.h"

scara_t scara;

#ifdef COMMUNICATION_UART

static uint8_t uart_receive_buff[MESSAGE_MAX_LENGTH];
static uint8_t uart_rec_buff_offset = 0;
#endif

char parsed[MSG_MAX_NUM_OF_PARAMETERS][MSG_MAX_LENGTH_OF_PARAMETER];
uint8_t message_words = 0; // Length of incomed message, also included first word as a Message head

// Reads command
// Parses command
// Command apply
void message_read(void)
{

#ifdef COMMUNICATION_UART

	xSemaphoreTake(getUartMutex(), portMAX_DELAY);
	int8_t receive_success = HAL_UART_Receive(&huart2, uart_receive_buff + uart_rec_buff_offset, 1, 5);
	xSemaphoreGive(getUartMutex());

	// @TODO: Reduce receiving timeot
	if (receive_success == HAL_OK)
	{
		// If message is too long, alarm the head device and read this message till the end (\n)
		if (uart_rec_buff_offset == MESSAGE_MAX_LENGTH)
		{
			message_remove_invalid_ch(); // Read all next characters till the \n
		}

		else
		{
			if (uart_receive_buff[uart_rec_buff_offset] == MESSAGE_END_CH)
			{
				uart_receive_buff[uart_rec_buff_offset] = '\0';

				message_parse();
				message_command_apply();
				uart_rec_buff_offset = 0; //Reset offset, and get ready for next message
			}

			else
			{
				uart_rec_buff_offset++;
			}
		}
	}
#endif
}

void message_command_apply(void)
{
	double angle = 0;
	char msg[20] = "";

	if (strcmp(parsed[0], "SCARATEST") == 0)
	{
		if (strcmp(parsed[1], "MOUTER") == 0)
		{
			angle = atof(parsed[2]); // @TODO Change atof

			scara.motor_outer.reference = (int32_t) (MOUTER_D_TO_INC * angle);
			scara.motor_outer.state = TEST;

			snprintf(msg, sizeof(msg), "%.2f\n\r", angle);
			xSemaphoreTake(getUartMutex(), portMAX_DELAY);
			HAL_UART_Transmit(&huart2, msg, sizeof(msg), 10);
			xSemaphoreGive(getUartMutex());

			// give command to regulator to start positioning
			// give command to regulator to log data
		}
	}
}

// Parsing message to words, and putting parsed words into variable parsed[][]
// Return: Number of words in message, also included first word which is message head
uint8_t message_parse(void)
{
	char* token = strtok(uart_receive_buff, MESSAGE_DELIMITER);
	uint8_t num_of_message_params = 0;

//	for (uint8_t i = 0; i < MSG_MAX_NUM_OF_PARAMETERS; i++)
//	{
//		memset(parsed[i], 0, strlen(parsed[i]));
//	}

	while (token)
	{
		strcpy(parsed[num_of_message_params++], token);

		xSemaphoreTake(getUartMutex(), portMAX_DELAY);
		HAL_UART_Transmit(&huart2, parsed[num_of_message_params - 1], sizeof(parsed[num_of_message_params - 1]), 10);
		xSemaphoreGive(getUartMutex());

		token = strtok(NULL, MESSAGE_DELIMITER);
	}

	xSemaphoreTake(getUartMutex(), portMAX_DELAY);
	HAL_UART_Transmit(&huart2, "\n\r", sizeof('\n\r'), 10);
	xSemaphoreGive(getUartMutex());

	return num_of_message_params;
}

// Removes characters from invalid message
// @TODO test this
void message_remove_invalid_ch(void)
{
	unsigned char msg[30] = "Message length exceeded!\r\n";

	xSemaphoreTake(getUartMutex(), portMAX_DELAY);
	HAL_UART_Transmit(&huart2, msg, sizeof(msg), 10);
	xSemaphoreGive(getUartMutex());

	do
	{
		HAL_UART_Receive(&huart2, uart_receive_buff, 1, 5);
	} while (uart_receive_buff[0] != MESSAGE_END_CH);

}
