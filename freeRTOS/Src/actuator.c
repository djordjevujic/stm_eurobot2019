/*
 * actuator.c
 *
 *  Created on: Mar 2, 2019
 *      Author: djox
 */
#include "actuator.h"


void pololu_set_reference(pololu_t* motor, uint8_t option, int32_t ref)
{
	if(option == MOUTER)
		motor->reference = ref;
}
