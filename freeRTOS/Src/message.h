/*
 * message.h
 *
 *  Created on: Feb 28, 2019
 *      Author: djox
 */

#ifndef MESSAGE_H_
#define MESSAGE_H_

#include "parser.h"
#include "usart.h"

#define COMMUNICATION_UART 1

#define MAX_MESSAGE_LENGTH 50

void message_read(void);
void message_parse();


#endif /* MESSAGE_H_ */
