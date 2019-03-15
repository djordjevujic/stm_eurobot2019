/*
 * limit_switch.c
 *
 *  Created on: Mar 5, 2019
 *      Author: djox
 */

#include "limit_switch.h"
#include "main.h"
#include "actuator.h"
#include "regulation.h"

scara_t scara;

static uint8_t mouter_lsw_cw_flag = 0;
static uint8_t mouter_lsw_ccw_flag = 0;

// This is called by LimitSwitchEXINT which is FreeRTOS task
//TODO Set task priority to very high
void limit_switch_handle(void)
{
	if (mouter_lsw_cw_flag == 1)
	{
		//Check state of pololu motor
		if (scara.motor_outer.state == CALIBRATING)
		{
			// Motor stop
			pololu_stop(&(scara.motor_outer), MOUTER);
			pololu_set_position(&(scara.motor_outer), MOUTER_CW_MAX);
			scara.motor_outer.state = IDLE; // Change state to IDLE
		}
		else if(scara.motor_outer.state == GOING_TO_POS || scara.motor_outer.state == TEST)
		{	// If motor is not calibrating, but it reaches LSW
			// This is problem!
			pololu_stop(&(scara.motor_outer), MOUTER);
			pololu_set_position(&(scara.motor_outer), MOUTER_CW_MAX);
			scara.motor_outer.state = OUT_OF_POS;			// Change state to IDLE
			// TODO Send message to main board about out of POS
		}
		mouter_lsw_cw_flag = 0;
	}
	else if (mouter_lsw_ccw_flag == 1)
	{
		//Check state of pololu motor
		if (scara.motor_outer.state == CALIBRATING)
		{
			// Motor stop
			pololu_stop(&(scara.motor_outer), MOUTER);
			pololu_set_position(&(scara.motor_outer), MOUTER_CCW_MAX);
			scara.motor_outer.state = IDLE;			// Change state to IDLE
		}
		else if (scara.motor_outer.state == GOING_TO_POS || scara.motor_outer.state == TEST)
		{	// If motor is not calibrating, but it reaches LSW
			// This is problem!
			pololu_stop(&(scara.motor_outer), MOUTER);
			pololu_set_position(&(scara.motor_outer), MOUTER_CCW_MAX);
			scara.motor_outer.state = OUT_OF_POS;					// Change state to IDLE
			// TODO Send message to main board about out of POS
			// TODO SEND WARNING BECAUSE OF WRONG ROTATION DIRECTION
		}
		mouter_lsw_ccw_flag = 0;
	}
}

// This is called by GPIO ex interrupt
void set_lsw_flag(uint16_t GPIO_PIN)
{
	switch (GPIO_PIN)
	{
	case MOUTER_LSW_CW_Pin:
	{
		mouter_lsw_cw_flag = 1;
		break;
	}

	case MOUTER_LSW_CCW_Pin:
		mouter_lsw_ccw_flag = 1;
		break;

	default:
	{
		//@ NOTE TODO Handle this situation by unknown LSW trigger indicator
		break;
	}
	}
}
