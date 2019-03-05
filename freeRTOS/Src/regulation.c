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
#include <limits.h>
#include "regulation.h"

scara_t scara;

void regulator_init(void)
{
	pololu_init();

}

void regulator(void)
{
	char msg[50] = "";
	int32_t count = 0; //@TODO: Think about to use int16_t type
	int32_t u = 0;

	count = __HAL_TIM_GET_COUNTER(scara.motor_outer.timer_enc);
	__HAL_TIM_SET_COUNTER(scara.motor_outer.timer_enc, DEFAULT_ENC_COUNTER_VALUE);

	count = count - DEFAULT_ENC_COUNTER_VALUE;

	scara.motor_outer.position += count;

	// Regulation of Scara outer motor position
	switch (scara.motor_outer.state)
	{
	case IDLE:
	{
		break;
	}

	case CALIBRATING:
	{
		pololu_set_dir(&(scara.motor_outer), DIRECTION_CW);
		calibrate();
		break;
	}

	case TEST:
	{
		if (scara.motor_outer.error > 0)
			scara.motor_outer.direction = DIRECTION_CW;
		else if (scara.motor_outer.error < 0)
			scara.motor_outer.direction = DIRECTION_CCW;

		if (scara.motor_outer.direction != scara.motor_outer.prev_direction)
		{
			// Flag will be set to 0 at pololu_change_dir() after changing direction
			// finished
			scara.motor_outer.changing_dir_flag = 1;
			u = 0;
		}

		if (scara.motor_outer.changing_dir_flag == 0)
		{
			u = reg_output_calc(&(scara.motor_outer), MOUTER_KP, 0, 0);
			u = sabs32(u);
			if (u > MOUTER_MAX_PWM)
				u = MOUTER_MAX_PWM;
		}

		else
		{
			pololu_change_dir(&(scara.motor_outer));
		}

		__HAL_TIM_SetCompare(scara.motor_outer.timer_pwm, TIM_CHANNEL_1, u); //@NOTE: Check channel if changing board

		scara.motor_outer.prev_direction = scara.motor_outer.direction;

		break;
	}
	default:
	{
		strcpy(msg, "WARNING: Unknown state of outer motor\n\r");
		xSemaphoreTake(getUartMutex(), portMAX_DELAY);
		HAL_UART_Transmit(&huart2, msg, sizeof(msg), 5);
		xSemaphoreGive(getUartMutex());
		break;
	}
	}

}

// For now, P regulator for position
int32_t reg_output_calc(pololu_t* motor, float kp, float ki, float kd)
{
	int32_t u = 0;

	motor->error = motor->reference - motor->position;

	u = (int32_t) (MOUTER_KP * motor->error);

	return u;
}

void calibrate()
{
	// Start calibration
	// MOUTER PWM
	// If switches are not pressed
	/*	if(HAL_GPIO_ReadPin(MOUTER_LSW_CW_GPIO_Port, MOUTER_LSW_CW_Pin) == 0
	 && HAL_GPIO_ReadPin(MOUTER_LSW_CCW_GPIO_Port, MOUTER_LSW_CCW_Pin) == 0)
	 __HAL_TIM_SetCompare(scara.motor_outer.timer_pwm, TIM_CHANNEL_1, CALIBRATING_PWM); //@NOTE: Check channel if changing board
	 else
	 {

	 }*/
	if (HAL_GPIO_ReadPin(MOUTER_LSW_CW_GPIO_Port, MOUTER_LSW_CW_Pin) == GPIO_PIN_SET)
	{
		pololu_set_position(&(scara.motor_outer), MOUTER_CW_MAX);
		pololu_set_dir(&(scara.motor_outer),DIRECTION_CCW);
		scara.motor_outer.state = IDLE;
	}

	else if(HAL_GPIO_ReadPin(MOUTER_LSW_CCW_GPIO_Port, MOUTER_LSW_CCW_Pin) == GPIO_PIN_SET)
	{
		pololu_set_position(&(scara.motor_outer), MOUTER_CCW_MAX);
		pololu_set_dir(&(scara.motor_outer),DIRECTION_CW);
		scara.motor_outer.state = IDLE;
	}

	// If motor is not on any LSW, go in one direction
	// When motor reaches LSW, it is handled by extern interrupt.
	// See function limit_switch_handle() in limit_switch.c
	else
	{
		pololu_set_dir(&(scara.motor_outer),DIRECTION_CW);
		__HAL_TIM_SetCompare(scara.motor_outer.timer_pwm, TIM_CHANNEL_1, CALIBRATING_PWM); //@NOTE: Check channel if changing board
	}
}

void pololu_init(void)
{
// OUTER Motor initialization
	scara.motor_outer.INA_Pin = MOUTER_INA_Pin;
	scara.motor_outer.INB_Pin = MOUTER_INB_Pin;
	scara.motor_outer.timer_enc = &htim1; // @NOTE Check this if changing controller/board
	scara.motor_outer.timer_pwm = &htim2;

	scara.motor_outer.state = IDLE;
	scara.motor_outer.position = 0;
	scara.motor_outer.error = 0;
	scara.motor_outer.direction = DIRECTION_CW;
	scara.motor_outer.prev_direction = DIRECTION_CW;
	scara.motor_outer.changing_dir_flag = 0;
	scara.motor_outer.INA_State = GPIO_PIN_RESET;
	scara.motor_outer.INB_State = GPIO_PIN_SET;

// Clockwise: INA->HIGH, INB->LOW
	HAL_GPIO_WritePin(GPIOA, scara.motor_outer.INA_Pin, scara.motor_outer.INA_State); // @NOTE Check port if changing board
	HAL_GPIO_WritePin(GPIOA, scara.motor_outer.INB_Pin, scara.motor_outer.INB_State); //@NOTE Check port if changing board

	__HAL_TIM_SET_COUNTER(scara.motor_outer.timer_enc, DEFAULT_ENC_COUNTER_VALUE);

}

#define BRAKING_INTERVAL 10 //Example: 10 * 1ms = 10 ms


// TODO Move to actuator.c
void pololu_set_dir(pololu_t* motor, uint8_t direction)
{
	if (direction == DIRECTION_CW)
	{
		motor->direction = DIRECTION_CW;
		motor->INA_State = GPIO_PIN_RESET;
		motor->INB_State = GPIO_PIN_SET;
	}

	else
	{
		motor->direction = DIRECTION_CCW;
		motor->INA_State = GPIO_PIN_SET;
		motor->INB_State = GPIO_PIN_RESET;
	}

	HAL_GPIO_WritePin(GPIOA, motor->INA_Pin, motor->INA_State); // @NOTE Check port if changing board
	HAL_GPIO_WritePin(GPIOA, motor->INB_Pin, motor->INB_State); //@NOTE Check port if changing board
}

// TODO Move to actuator.c
void pololu_change_dir(pololu_t* motor)
{
	static uint8_t braking_cnt = 0;

	if (braking_cnt == 0)
	{
		motor->INA_State = GPIO_PIN_RESET;
		motor->INB_State = GPIO_PIN_RESET;
	}

	if (braking_cnt < BRAKING_INTERVAL)
	{
		braking_cnt++;
	}

// If braking interval passed, change directions
	if (braking_cnt == BRAKING_INTERVAL)
	{
		if (motor->direction == DIRECTION_CW)
		{
			motor->INA_State = GPIO_PIN_RESET;
			motor->INB_State = GPIO_PIN_SET;
		}

		else
		{
			motor->INA_State = GPIO_PIN_SET;
			motor->INB_State = GPIO_PIN_RESET;
		}

		braking_cnt = 0;
		motor->changing_dir_flag = 0;
	}

	HAL_GPIO_WritePin(GPIOA, motor->INA_Pin, motor->INA_State); // @NOTE Check port if changing board
	HAL_GPIO_WritePin(GPIOA, motor->INB_Pin, motor->INB_State); //@NOTE Check port if changing board
}

int32_t sabs32(int32_t i)
{
	int32_t res;

	if (INT32_MIN == i)
	{
		res = INT32_MAX;
	}
	else
	{
		res = i < 0 ? -i : i;
	}

	return res;
}
