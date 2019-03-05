/*
 * limit_switch.c
 *
 *  Created on: Mar 5, 2019
 *      Author: djox
 */

#include "limit_switch.h"
#include "main.h"
#include "actuator.h"

scara_t scara;

static uint8_t mouter_lsw_flag = 0;

// This is called by LimitSwitchEXINT which is FreeRTOS task
//TODO Set task priority to very high
void limit_switch_handle(void)
{
	if(mouter_lsw_flag == 1)
	{
		//Check state of pololu motor
		if(scara.motor_outer.state == CALIBRATING)
		{
			//TODO CHECK IF PINS ARE ALREADY SET WHEN STARTING CALIBRATION!
			// Motor stop
			// Change state to IDLE
			// Set angle
		}
		else
		{
			// This is problem!
			// Motor stop
			// Change state tp OUT_OF_POS
		}
		mouter_lsw_flag = 0;
	}
}

// This is called by GPIO ex interrupt
void set_lsw_flag(uint16_t GPIO_PIN)
{
	switch (GPIO_PIN)
	{
		case MOUTER_LSW_CW_Pin:
		{
			mouter_lsw_flag = 1;
			break;
		}

		case MOUTER_LSW_CCW_Pin:
			mouter_lsw_flag = 1;
			break;

		default:
		{
			//@ NOTE TODO Handle this situation by unknown LSW trigger indicator
			break;
		}
	}
}
