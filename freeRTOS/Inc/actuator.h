/*
 * actuator.h
 *
 *  Created on: Mar 2, 2019
 *      Author: djox
 */

#ifndef ACTUATOR_H_
#define ACTUATOR_H_

#include <stdint.h>
#include "stm32f4xx_hal.h"
#include "tim.h"
/*		SCARA CONFIGURATION - bird's eye view
 *
 * 				outer
 * 				motor		Vacuum
 * 					C------o
 *			 	 /
 *			  /
 *			 /
 *			/
 *		 C inner
 *		 	 motor
 */
#define DIRECTION_CW 1
#define DIRECTION_CCW 0

#define DEFAULT_ENC_COUNTER_VALUE 20000U /* This value will be initialised to encoder timer every time after reading increments */

// Unicate constants for each motor
#define MOUTER 1 // Outer motor of scara
#define MOUTER_CW_MAX 90.0f
#define MOUTER_CCW_MAX -90.0f

#define SET_REFERENCE 1

typedef enum pololuState {
	IDLE,
	STOP,
	GOING_TO_POS,
	TEST,
	CALIBRATING,
	OUT_OF_POS
}pololuState;

/*
 * @TODO - Add ports from INA and INB to the struct
 */
typedef struct pololu_t
{
	volatile int32_t position;
	volatile int32_t error;
	volatile int32_t reference;
	volatile uint8_t direction;
	volatile uint8_t prev_direction;
	volatile uint8_t changing_dir_flag;
	uint16_t INA_Pin;
	uint16_t INB_Pin;
	GPIO_PinState INA_State;
	GPIO_PinState INB_State;

	pololuState state;
	TIM_HandleTypeDef* timer_enc;
	TIM_HandleTypeDef* timer_pwm;
}pololu_t;

typedef struct scara
{
	pololu_t motor_outer;
}scara_t;

void pololu_set_position(pololu_t* motor, float angle);



#endif /* ACTUATOR_H_ */
