/*
 * LivingFlameThrower.c
 *
 *  Created on: 2014-11-27
 *      Author: brendan
 */
#include "LivingFlameThrower.h"

/* Use FireBreath trait */
static int flames(FireBreath *trait)
{
	LivingFlameThrower *this = (LivingFlameThrower *) CastTrait(trait);
	return this->data._fuelPower;
}

Constructor(LivingFlameThrower)
{
	LinkTrait(FireBreath);
	LinkTraitMethod(FireBreath, flames);

	this->data._fuelPower = 21;
}
