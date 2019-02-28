/*
 * message.h
 *
 *  Created on: Feb 28, 2019
 *      Author: djox
 */

#ifndef MESSAGE_H_
#define MESSAGE_H_

#include "config.h"
#include "usart.h"

#define MESSAGE_MAX_LENGTH 	100 //MSG_MAX_NUM_OF_PARAMETERS * MSG_MAX_LENGTH_OF_PARAMETER
#define MESSAGE_END_CH  		'\n'
#define MESSAGE_DELIMITER 	" "

#define MSG_MAX_NUM_OF_PARAMETERS 	10
#define MSG_MAX_LENGTH_OF_PARAMETER 10

void message_read(void);
uint8_t message_parse();
void message_remove_invalid_ch(void); // Removes invalid characters from message

#endif /* MESSAGE_H_ */
