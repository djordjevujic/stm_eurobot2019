/*
 * regulation.c
 *
 *  Created on: Mar 1, 2019
 *      Author: djox
 */

/* OUTER_MOTOR
 *     NUCLEO F401
 * INA
 * INB
 *
 */
#include "regulation.h"

scara_t scara;

void regulator_init(void)
{
	pololu_init();
}

void regulator(void)
{
	// Regulation of Scara outer motor position

}

uint16_t reg_output_calc(pololu_t* motor, float kp, float ki, float kd)
{

}


 void pololu_init(void)
{
	// OUTER Motor initialization
	scara.motor_outer.INA_Pin = MOUTER_INA_Pin;
	scara.motor_outer.INB_Pin = MOUTER_INB_Pin;
	scara.motor_outer.timer = &htim1; // @NOTE Check this if changing controller/board

	scara.motor_outer.state = IDLE;
	scara.motor_outer.u = 0;
	scara.motor_outer.position = 0;
	scara.motor_outer.error = 0;
	scara.motor_outer.direction = DIRECTION_CW;
	scara.motor_outer.prev_direction = DIRECTION_CW;
	scara.motor_outer.INA_State = GPIO_PIN_SET;
	scara.motor_outer.INB_State = GPIO_PIN_RESET;

	// Clockwise: INA->HIGH, INB->LOW
	HAL_GPIO_WritePin(GPIOA, scara.motor_outer.INA_Pin, scara.motor_outer.INA_State); // @NOTE Check port if changing board
	HAL_GPIO_WritePin(GPIOA, scara.motor_outer.INB_Pin, scara.motor_outer.INB_State); //@NOTE Check port if changing board

	__HAL_TIM_SET_COUNTER(scara.motor_outer.timer, DEFAULT_ENC_COUNTER_VALUE);

}


