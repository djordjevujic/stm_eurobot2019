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

#define DEFAULT_ENC_COUNTER_VALUE 20000 /* This value will be initialised to encoder timer every time after reading increments */

// Unicate constant for each motor
#define MOUTER 1 // Outer motor of scara

// Outer motor configuration
#define MOUTER_D_TO_INC 9.977333
#define MOUTER_INC_TO_D 0.1002271816

#define SET_REFERENCE 1

typedef enum pololuState {
	IDLE,
	STOP,
	GOING_TO_POS,
	CALIBRATING
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
	volatile uint16_t u;
	uint16_t INA_Pin;
	uint16_t INB_Pin;
	GPIO_PinState INA_State;
	GPIO_PinState INB_State;

	pololuState state;
	TIM_HandleTypeDef* timer;
}pololu_t;

typedef struct scara
{
	pololu_t motor_outer;
}scara_t;

void pololu_set_reference(pololu_t* motor, uint8_t option, int32_t ref);


#endif /* ACTUATOR_H_ */
