/*
 * message.c
 *
 *  Created on: Feb 28, 2019
 *      Author: djox
 */
#include "message.h"

static uint8_t uart_receive_buff[MAX_MESSAGE_LENGTH];
static uint8_t uart_rec_buff_offset = 0;
uint8_t msg[20] = ""; // @TODO Delete later


void message_read(void)
{

#ifdef COMMUNICATION_UART

	// @TODO: Reduce receiving timeot
	if (HAL_UART_Receive(&huart2, uart_receive_buff + uart_rec_buff_offset, 1,
			5) == HAL_OK)
	{
		// If message is too long, alarm the head device
		if (uart_rec_buff_offset == MAX_MESSAGE_LENGTH)
			HAL_UART_Transmit(&huart2, "Message length exceeded!\r\n",
					sizeof("Message length exceeded!\r\n"), 10);

		else
		{
			if (uart_receive_buff[uart_rec_buff_offset] == '\n')
			{
				uart_receive_buff[uart_rec_buff_offset] = '\0';

				//msg_parse(uart_receive_buff);

				uart_rec_buff_offset = 0;
				strcpy(msg, uart_receive_buff);
				HAL_UART_Transmit(&huart2, msg, sizeof(msg), 10);

			}

			else
				uart_rec_buff_offset++;
		}
	}
#endif
}

void message_parse()
{

}
