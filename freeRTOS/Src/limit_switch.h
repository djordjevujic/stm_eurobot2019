/*
 * limit_switch.h
 *
 *  Created on: Mar 5, 2019
 *      Author: djox
 */

#ifndef LIMIT_SWITCH_H_
#define LIMIT_SWITCH_H_

#include <stdint.h>

void limit_switch_handle(void);
void set_lsw_flag(uint16_t GPIO_PIN);

#endif /* LIMIT_SWITCH_H_ */
