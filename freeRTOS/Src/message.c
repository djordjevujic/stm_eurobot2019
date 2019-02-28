/*
 * message.c
 *
 *  Created on: Feb 28, 2019
 *      Author: djox
 */

//@TODO Message words handling
//@TODO Resolve WARNINGS!

#include <string.h>

#include "message.h"
#include "config.h"

#ifdef COMMUNICATION_UART
static uint8_t uart_receive_buff[MESSAGE_MAX_LENGTH];
static uint8_t uart_rec_buff_offset = 0;
#endif

volatile char parsed[MSG_MAX_NUM_OF_PARAMETERS][MSG_MAX_LENGTH_OF_PARAMETER];
uint8_t message_words = 0; // Length of incomed message, also included first word as a Message head
uint8_t const msg_delim[2] = " ";

void message_read(void)
{

#ifdef COMMUNICATION_UART

	// @TODO: Reduce receiving timeot
	if (HAL_UART_Receive(&huart2, uart_receive_buff + uart_rec_buff_offset, 1, 5) == HAL_OK)
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

				message_parse(uart_receive_buff);

				uart_rec_buff_offset = 0; //Reset offset, and get ready for next message

				//@TODO This is only for testing, delete later
				//strcpy(msg, uart_receive_buff);
				//HAL_UART_Transmit(&huart2, uart_receive_buff, sizeof(uart_receive_buff), 10);

			}

			else
				uart_rec_buff_offset++;
		}
	}
#endif
}

// Parsing message to words, and putting parsed words into variable parsed[][]
// Return: Number of words in message, also included first word which is message head
uint8_t message_parse(void)
{
	char* token = strtok(uart_receive_buff, msg_delim);
	uint8_t num_of_message_params = 0;

	//token = strtok(uart_receive_buff, MESSAGE_DELIMITER);

	while (token)
	{
		strcpy(parsed[num_of_message_params++], token);

		//HAL_UART_Transmit(&huart2, parsed[num_of_message_params-1], sizeof(num_of_message_params-1), 10);

		token = strtok(NULL, msg_delim);
	}

	return num_of_message_params;
}
// Removes characters from invalid message
	void message_remove_invalid_ch(void)
	{

		HAL_UART_Transmit(&huart2, "Message length exceeded!\r\n", sizeof("Message length exceeded!\r\n"), 10);

		do
		{
			HAL_UART_Receive(&huart2, uart_receive_buff, 1, 5);
		} while (uart_receive_buff[0] != MESSAGE_END_CH);

	}
