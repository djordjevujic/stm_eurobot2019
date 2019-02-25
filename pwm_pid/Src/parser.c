/*
 * parser.c
 *
 *  Created on: Feb 24, 2019
 *      Author: djox
 */
#include "../Inc/parser.h"

uint8_t parsed_msg[MAX_NUM_OF_PARAMETERS][MAX_LENGTH_OF_PARAMETER] = { "", "",
		"", "", "", "", "", "", "", "" };
static const uint8_t delim[2] = " ";
static uint8_t* token = strtok(msg, delim);
uint8_t num_of_params = 0;

msg_parse(uint8_t* msg) {
	//HAL_UART_Transmit(&huart2, "PARSER", 6, 50);
	uint8_t* token = strtok(msg, delim);

	while (token) {

		strcpy(parsed[num_of_params++], token);

		token = strtok(NULL, " ");
	}

	if (strcmp(parsed[0], "test") == 0) {

	}

}
