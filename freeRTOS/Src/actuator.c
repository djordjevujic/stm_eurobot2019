/*
 * actuator.c
 *
 *  Created on: Mar 2, 2019
 *      Author: djox
 */
#include "actuator.h"
#include "regulation.h"

void pololu_set_position(pololu_t* motor, float angle)
{
	motor->position = (int32_t) (MOUTER_D_TO_INC * angle);
}

