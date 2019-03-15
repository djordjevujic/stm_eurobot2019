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
#include "usart.h"

extern scara_t scara;

// Outer motor configuration
#define MOUTER_KP 0.55f
#define MOUTER_MAX_PWM 1049U
#define MOUTER_D_TO_INC 9.977333f
#define MOUTER_INC_TO_D 0.1002271816f

#define CALIBRATING_PWM 300U

void regulator(void);
void regulator_init(void);

int32_t reg_output_calc(pololu_t* motor, float kp, float ki, float kd);
void pololu_init(void);
int32_t sabs32(int32_t i);
void pololu_change_dir(pololu_t* motor);
void pololu_set_dir(pololu_t* motor, uint8_t direction);
void calibrate(pololu_t* motor, uint8_t motor_param);


#endif /* REGULATION_H_ */
