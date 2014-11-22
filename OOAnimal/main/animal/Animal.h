/* Animal class header
 * Copyright (C) 2014 Brendan Bruner
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * bbruner@ualberta.ca
 *
 *  Created on: 2014-11-19
 *      Author: brendan
 */

#ifndef ANIMAL_H_
#define ANIMAL_H_

/******************************************************************************
 * Class Animal
 *****************************************************************************/
/**
 * Animal
 * Describes how an animal talks, how many legs it has, and where it lives.
 */
typedef struct Animal Animal;

/******************************************************************************
 * vtable prototype
 *****************************************************************************/
struct vtable_Animal
{
	char * (* getName)(Animal *);
	void (* talk)(Animal *);
	void (* setLegs)(Animal *, int);
	int (* getLegs)(Animal *);
	void (* location)(Animal *);
};

/******************************************************************************
 * Class data members
 *****************************************************************************/
/**
 * @param numLegs How many legs the animal has.
 * @param name A pointer the Animal's name (ie, a string). This means when the
 * Animal's name is set, the string used to set it is not copied. So.. be
 * careful the string being referenced does not dissapear off the stack or
 * something.
 */
struct data_Animal
{
	int numLegs;
	char *name;
};

/******************************************************************************
 * Class data structure
 *****************************************************************************/
struct Animal
{
	struct vtable_Animal vtable;
	struct data_Animal _data;
};

/******************************************************************************
 * Constructor and destructor prototypes
 *****************************************************************************/
/**
 * Constructor.
 * Initializes an object.
 * @param this Object to initialize.
 * @param name The animals name.
 */
void new_Animal(Animal *this, char const *name);

/**
 * Destructor.
 * @param this The object to destroy.
 */
void destroy_Animal(Animal *this);

#endif /* ANIMAL_H_ */
