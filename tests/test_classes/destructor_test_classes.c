/*
 * Copyright 2015 Brendan Bruner
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * bbruner@ualberta.ca
 * Jun 19, 2016
 */

#include <test_classes/destructor_test_classes.h>


/****************************************************************************/
/* Class A																	*/
/****************************************************************************/
const struct DTClassA_VTable* DTClassA_VTable_Create( )
{
	static struct DTClassA_VTable vtable;
	vtable.CObject_VTable = *CObject_VTable_Create( );
	return &vtable;
}
void newDTClassA( struct DTClassA* self, int* testVar )
{
	CAssertObject(self);
	CAssertObject(testVar);

	/* Call super's constructor. */
	CObject(&self->cobject);

	/* Map vtable. */
	CVTable(self, DTClassA_VTable_Create( ));

	/* Set test var to zero. */
	*testVar = 0;
	self->destructorTestVar = testVar;
}


/****************************************************************************/
/* Class B																	*/
/****************************************************************************/
const struct DTClassB_VTable* DTClassB_VTable_Create( )
{
	static struct DTClassB_VTable vtable;
	vtable.DTClassA_VTable = *DTClassA_VTable_Create( );
	return &vtable;
}
void newDTClassB( struct DTClassB* self, int* testVar )
{
	CAssertObject(self);
	CAssertObject(testVar);

	/* Call super's constructor. */
	newDTClassA(&self->dtClassA, testVar);

	/* Map vtable. */
	CVTable(self, DTClassB_VTable_Create( ));
}


/****************************************************************************/
/* Class C																	*/
/****************************************************************************/
static void dtClassCDestroy( void* self_ )
{
	/* This is DTClassC's implementation, so cast object to type DTClassC. */
	struct DTClassC* self = CCast(self_);

	/* Increment test var */
	++*(self->dtClassA).destructorTestVar;

	/* Call super's destructor. */
	void (*dest)( void* );
	struct DTClassC_VTable* vtable;
	vtable = CGetVTable(self, struct DTClassC_VTable);
	dest = vtable->Supers_DTClassA_VTable->CObject_VTable.CDestructor;
	dest(self);
}

const struct DTClassC_VTable* DTClassC_VTable_Create( )
{
	static struct DTClassC_VTable vtable =
	{	   
		.DTClassA_VTable =
		{
			.CObject_VTable =
			{
				.CDestructor = dtClassCDestroy
			}
		},
	};
	vtable.Supers_DTClassA_VTable = DTClassA_VTable_Create( );
	return &vtable;
}
void newDTClassC( struct DTClassC* self, int* testVar )
{
	CAssertObject(self);
	CAssertObject(testVar);

	/* Call super's constructor. */
	newDTClassA(&self->dtClassA, testVar);

	/* Remap super's vtable and assign self's vtable. */
	CVTable(self, DTClassC_VTable_Create( ));
}


/****************************************************************************/
/* Class D																	*/
/****************************************************************************/
static void dtClassDDestroy( void* self_ )
{
	/* This is DTClassD's implementation, so cast object to type DTClassD. */
	struct DTClassD* self = CCast(self_);

	/* Increment test var. */
	++*(self->dtClassB.dtClassA).destructorTestVar;

	/* Call super's destructor. */
	CGetVTable(self, struct DTClassD_VTable)->Supers_DTClassB_VTable->DTClassA_VTable.CObject_VTable.CDestructor(self);
}

const struct DTClassD_VTable* DTClassD_VTable_Create( )
{
	static struct DTClassD_VTable vtable =
	{
		.DTClassB_VTable =
		{
			.DTClassA_VTable =
			{
				.CObject_VTable =
				{
					.CDestructor = dtClassDDestroy
				}
			}
		},
	};
	vtable.Supers_DTClassB_VTable = DTClassB_VTable_Create( );
	return &vtable;
}
void newDTClassD( struct DTClassD* self, int* testVar )
{
	CAssertObject(self);
	CAssertObject(testVar);

	/* Call super's constructor. */
	newDTClassB(&self->dtClassB, testVar);

	/* Map vtable. */
	CVTable(self, DTClassD_VTable_Create( ));
}


/****************************************************************************/
/* Class E																	*/
/****************************************************************************/
static void dtClassEDestroy( void* self_ )
{
	/* This is DTClassE's implementation, so cast object to type DTClassE. */
	struct DTClassE* self = CCast(self_);

	/* Set test var. */
	*(self->dtClassC.dtClassA).destructorTestVar = DT_CLASS_E_VAL;

	/* Call super's destructor. */
	CGetVTable(self, struct DTClassE_VTable)->Supers_DTClassC_VTable->DTClassA_VTable.CObject_VTable.CDestructor(self);
}

const struct DTClassE_VTable* DTClassE_VTable_Create( )
{
	static struct DTClassE_VTable vtable;
	vtable.DTClassC_VTable = *DTClassC_VTable_Create( );
	vtable.Supers_DTClassC_VTable = DTClassC_VTable_Create( );

	((struct CObject_VTable*) &vtable)->CDestructor = dtClassEDestroy;
//	vtable.DTClassC_VTable.DTClassA_VTable.CObject_VTable.CDestructor = dtClassEDestroy;
	return &vtable;
}

void newDTClassE( struct DTClassE* self, int* testVar )
{
	CAssertObject(self);
	CAssertObject(testVar);

	/* Call super's constructor. */
	newDTClassC(&self->dtClassC, testVar);

	/* Override destructor. */
	CVTable(self, DTClassE_VTable_Create( ));
}
