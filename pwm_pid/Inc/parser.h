/*
 * parser.h
 *
 *  Created on: Feb 24, 2019
 *      Author: djox
 */

#ifndef PARSER_H_
#define PARSER_H_

#include <stdint.h>
#include "usart.h"
#include <string.h>

#define MAX_NUM_OF_PARAMETERS		10
#define MAX_LENGTH_OF_PARAMETER 	10

#define TEST 1

// Functions
void msg_parse(uint8_t* msg);

#endif /* PARSER_H_ */
