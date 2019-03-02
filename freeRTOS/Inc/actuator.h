/*
 * actuator.h
 *
 *  Created on: Mar 2, 2019
 *      Author: djox
 */

#ifndef ACTUATOR_H_
#define ACTUATOR_H_

#include <stdint.h>

/*		SCARA CONFIGURATION - bird's eye view
 *
 * 				outer
 * 				motor		Vacuum
 * 					C------	o
 *			 	 /
 *			  /
 *			 /
 *			/
 *		 c inner
 *		 	 motor
 */
#define DIRECTION_CW 1
#define DIRECTION_CCW 0

// Outer motor configuration
#define MOTOR_OUT_D_TO_INC 9.977333
#define MOTOR_OUT_INC_TO_D 0.1002271816


typedef struct dc_motor_enc
{
	//const uint16_t increments_per_circle;
	volatile int32_t position = 0;
	volatile int32_t error = 0;
	volatile int32_t reference = 0;
	volatile uint8_t direction = 0;
	static const float kp;
}dc_motor_enc_t;

typedef struct scara
{
	dc_motor_enc motor_outer;
}scara_t;

#endif /* ACTUATOR_H_ */
