/*
 * regulation.h
 *
 *  Created on: Mar 1, 2019
 *      Author: djox
 */

#ifndef REGULATION_H_
#define REGULATION_H_

#include <stdint.h>
#include "actuator.h"
#include "main.h"

extern scara_t scara;


void regulator(void);
void regulator_init(void);

uint16_t reg_output_calc(pololu_t* motor, float kp, float ki, float kd);
void pololu_init(void);

#endif /* REGULATION_H_ */
